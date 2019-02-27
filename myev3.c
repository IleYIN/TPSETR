/*
 * myev3.c
 *
 *  Created on: 26 janv. 2017
 *      Author: grolleau
 */
#include <ev3.h>
#include <ev3_port.h>
#include <ev3_tacho.h>
#include <ev3_sensor.h>
#include "myev3.h"
#include <stdio.h>
/**
 * Sequence Numbers (sn) obtained from the EV3 ports used by ev3dev-c
 */
uint8_t MY_US_SENSOR;
uint8_t MY_MINI_TACHO;
uint8_t MY_LEFT_TACHO;
uint8_t MY_RIGHT_TACHO;
/**
 * Initializes the "sequence numbers" (sn) used in ev3dev-c API from the EV3 ports
 * Call after initializing the ev3, ev3 ports, ev3 sensors, and ev3 tachos
 * \return 0 if ok, 1 if error (i.e. some sensor/actuator was not found)
 */
int my_init_ev3() {
	ev3_search_tacho_plugged_in(LEFT_TACHO_PORT, EXT_PORT__NONE_,&MY_LEFT_TACHO,0);
	if (get_tacho_type_inx(MY_LEFT_TACHO)!=LEGO_EV3_L_MOTOR) {
		printf("Cannot find left tacho-motor\n");
		return 1;
	} else {
		printf("Found left tacho-motor sn=%d\n",MY_LEFT_TACHO);
	}

	ev3_search_tacho_plugged_in(RIGHT_TACHO_PORT, EXT_PORT__NONE_,&MY_RIGHT_TACHO,0);
	if (get_tacho_type_inx(MY_RIGHT_TACHO)!=LEGO_EV3_L_MOTOR) {
		printf("Cannot find right tacho-motor\n");
		return 1;
	} else {
		printf("Found right tacho-motor sn=%d\n",MY_RIGHT_TACHO);
	}
	ev3_search_tacho_plugged_in(MINI_TACHO_PORT, EXT_PORT__NONE_,&MY_MINI_TACHO,0);
	if (get_tacho_type_inx(MY_MINI_TACHO)!=LEGO_EV3_M_MOTOR) {
		printf("Cannot find mini tacho\n");
		return 1;
	} else {
		printf("Found mini tacho-motor sn=%d\n",MY_MINI_TACHO);
	}
	ev3_search_sensor_plugged_in(US_SENSOR_PORT,EXT_PORT__NONE_,&MY_US_SENSOR,0);
	if (get_sensor_type_inx(MY_US_SENSOR)!=LEGO_NXT_US) {
		printf("Cannot find ultrasound sensorn");
		return 1;
	} else {
		printf("Found ultrasonic sensor sn=%d\n",MY_US_SENSOR);
	}

	return 0;
}
