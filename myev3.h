/*
 * myev3.h
 *
 *  Created on: 26 janv. 2017
 *      Author: grolleau
 */

#ifndef MYEV3_H_
#define MYEV3_H_
#include <ev3.h>
/**
 * EV3 ports where sensors/actuators are connected
 */
#define US_SENSOR_PORT INPUT_4
#define MINI_TACHO_PORT OUTPUT_D
#define LEFT_TACHO_PORT OUTPUT_B
#define RIGHT_TACHO_PORT OUTPUT_A

/**
 * Sequence Numbers (sn) obtained from the EV3 ports used by ev3dev-c
 */
extern uint8_t MY_US_SENSOR;
extern uint8_t MY_MINI_TACHO;
extern uint8_t MY_LEFT_TACHO;
extern uint8_t MY_RIGHT_TACHO;

/**
 * Modes
 */
#define MODE_DIRECT 0
#define MODE_AUTO 1
/**
 * Commands used by the auto and direct mode
 */
#define CMD_STOP 	0 	//!< Stops the current immediate or move to target command
#define CMD_FORWARD 1 	//!< Immediate command, go forward until stop command
#define CMD_BACKWARD 2	//!< Immediate command, go backward until stop command
#define CMD_LEFT 	3 	//!< Immediate command, turn to the right until stop command
#define CMD_RIGHT	4 	//!< Immediate command, turn to the right until stop command
#define CMD_START 	5	//!< Starts to move to the specified target, will stop when target reached or stop command

/**
 * Status of the robot
 */
#define STATUS_STANDBY 		0	//!< Waiting for command
#define STATUS_DIRECT_MOVE 	1	//!< Moving until stop command
#define STATUS_AUTO_MOVE 	2	//!< Moving to target

/**
 * Initializes the "sequence numbers" (sn) used in ev3dev-c API from the EV3 ports
 * Call after initializing the ev3, ev3 ports, ev3 sensors, and ev3 tachos
 * \return 0 if ok, 1 if error (i.e. some sensor/actuator was not found)
 */
int my_init_ev3();

//Strangely roundf and round are not declared in math.h, this declaration avoids a compilation warning
float roundf(float);
double round(double);
#endif /* MYEV3_H_ */
