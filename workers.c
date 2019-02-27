/*
 * workers.c
 *
 *  Created on: 26 janv. 2017
 *      Author: grolleau
 */
#include <ev3.h>
#include <ev3_port.h>
#include <ev3_tacho.h>
#include <ev3_sensor.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "myev3.h"

/**
 * Initializes the "sequence numbers" (sn) used in ev3dev-c API from the EV3 ports
 * \return 0 if ok, 1 if error (i.e. some sensor/actuator was not found)
 */
static int myreadUS(int nbshots) {
	int min = 255;
	int i;
	const uint8_t sn = MY_US_SENSOR;
	for (i = 0; i < nbshots; i++) {
		set_sensor_mode_inx(sn, LEGO_NXT_US_US_SI_CM);
		usleep(20000);
		float f;
		get_sensor_value0(sn, &f);
		f = roundf(f);
		min = min < f ? min : f;
	}
	return min;
}

static int oneScan (int a, int dir) {
	const uint8_t sn = MY_MINI_TACHO;
	int us;
	FLAGS_T flags;
	set_tacho_duty_cycle_sp(sn,dir*100);
	set_tacho_position_sp(sn, a);
	set_tacho_command_inx(sn, TACHO_RUN_TO_ABS_POS);
	do {
		get_tacho_state_flags(sn,&flags);
	} while (flags&TACHO_RUNNING);
	us = myreadUS(1);
	return us;
}

int min(int x,int y) {
	return x<y?x:y;
}

/**
 * Initializes the mini-tacho motor for the scanWorker, call it once \
 * 	before calling scanWorker
 */
void scanWorkerInit() {
	int speed;
	get_tacho_max_speed(MY_MINI_TACHO,&speed);
	set_tacho_command_inx(MY_MINI_TACHO,TACHO_RESET);
	set_tacho_speed_sp(MY_MINI_TACHO,speed/3);
	set_tacho_duty_cycle_sp(MY_MINI_TACHO,100);
	set_tacho_stop_action_inx(MY_MINI_TACHO, TACHO_HOLD);
}

/**
 * One iteration of scan with ultrasonic sensor
 * Requires: scanInit has been called once before
 * \param[out] obstacles: array of 5 integers where this \
 * 		function stores the value read on the ultrasonic sensor for angles \
 * 		-90° -45° 0° (ie., front) 45° 90° respectively
 */
void scanWorker(int *obstacles) {
	obstacles[2]=myreadUS(1);
	obstacles[1]=oneScan (-45, -1);
	obstacles[0]=oneScan (-90, -1);
	obstacles[1]=min(obstacles[1],oneScan (-45, 1));
	obstacles[2]=min(obstacles[2],oneScan(0, 1));
	obstacles[3]=oneScan (45, 1);
	obstacles[4]=oneScan (90, 1);
	obstacles[3]=min(obstacles[3],oneScan (45, -1));
	obstacles[2]=min(obstacles[2],oneScan(0, -1));
	printf("%d\n",obstacles[2]);
}

/**
 * Constants used for the deadreckoning
 */
#define DR_INTERSPACE 16.0
#define DR_PERIM 13.7

/**
 * Initializes the deadReckoning, call it once \
 * 	before calling deadRWorker
 */
void deadRWorkerInit() {
	set_tacho_command_inx(MY_LEFT_TACHO,TACHO_RESET);
	set_tacho_command_inx(MY_RIGHT_TACHO,TACHO_RESET);
	set_tacho_stop_action_inx (MY_LEFT_TACHO, TACHO_BRAKE);
	set_tacho_stop_action_inx (MY_RIGHT_TACHO, TACHO_BRAKE);
}

/**
 * On iteration of the deadReckoning
 * Requires: deadRWorkerInit has been called once before
 * \param[in] prevX previous value of x
 * \param[in] prevY previous value of y
 * \param[in] prevA previous value of alpha in RADIAN
 * \param[out] X new value of x
 * \param[out] Y new value of y
 * \param[out] A new value of alpha in RADIAN
 */
void deadRWorker(double prevX, double prevY, double prevA, double *X, double *Y, double* A) {
	static int prevRight=0;
	static int prevLeft=0;
	int tachoRight, tachoLeft;
	double distRight, distLeft;
	get_tacho_position(MY_LEFT_TACHO,&tachoRight);
	get_tacho_position(MY_RIGHT_TACHO,&tachoLeft);
	distRight=(DR_PERIM*(tachoRight-prevRight))/360.0;
	distLeft=(DR_PERIM*(tachoLeft-prevLeft))/360.0;
	prevRight=tachoRight;
	prevLeft=tachoLeft;
	if (fabs(distRight-distLeft)<1.0e-6) {
		*A=prevA;
		*X=prevX+cos(prevA)*distRight;
		*Y=prevY+sin(prevA)*distRight;
	} else {
	    double R = DR_INTERSPACE * (distLeft + distRight) / (2 * (distLeft-distRight));
	    double wd = (distLeft-distRight) / DR_INTERSPACE;
		*X=prevX+R*sin(wd+prevA)-R*sin(prevA);
		*Y=prevY-R*cos(wd+prevA)+R*cos(prevA);
		*A=prevA+wd;
		if (*A>M_PI)
			*A-=2*M_PI;
		else if (*A<-M_PI)
			*A+=2*M_PI;
	}
}

static int normalize(int value, int min, int max) {
	if (value>max) {
		return max;
	} else if (value<min) {
		return min;
	}
	return value;
}

/*
 * Does one iteration going to (x,y) in a straight line
 * \param[in] x,y,a current position and heading of the robot, a in radian
 * \param[in] targetX,targetY coordinates of the target
 * \parma[in] power maximum power used
 * \return the error in cm between (x,y) and (targetX,targetY)
 */
double deadreckoningGoTo(const double x, const double y, const double a, const double targetX, const double targetY, int power) {
	double gamma; // Heading angle of (x,y) compared to actual position
	const int gain=10;
	const int gaina=286;
	double error;
	double errora;
	double diffy=targetY-y;
	double diffx=targetX-x;
	gamma=atan2(diffy,diffx);
	errora=gamma-a;
	if (errora > M_PI) {
		errora-=M_PI;
	} else if (errora<=-M_PI) {
		errora+=M_PI;
	}
	error=sqrt(diffy*diffy+diffx*diffx);
	int cmdR,cmdL;
	int cmd=normalize(gain*error,0,power);
	cmdR=normalize(errora*gaina+cmd,-power,power);
	cmdL=normalize(-errora*gaina+cmd,-power,power);
	set_tacho_duty_cycle_sp(MY_RIGHT_TACHO,cmdR);
	set_tacho_duty_cycle_sp(MY_LEFT_TACHO,cmdL);
	set_tacho_command_inx(MY_RIGHT_TACHO,TACHO_RUN_DIRECT);
	set_tacho_command_inx(MY_LEFT_TACHO,TACHO_RUN_DIRECT);
	return error;
}

