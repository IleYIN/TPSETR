/*
 * workers.h
 *
 *  Created on: 26 janv. 2017
 *      Author: grolleau
 */

#ifndef WORKERS_H_
#define WORKERS_H_

/**
 * Initializes the mini-tacho motor for the scanWorker, call it once \
 * 	before calling scanWorker
 */
void scanWorkerInit();

/**
 * One iteration of scan with ultrasonic sensor
 * Requires: scanInit has been called once before
 * \param[out] obstacles: array of 5 integers where this \
 * 		function stores the value read on the ultrasonic sensor for angles \
 * 		-90° -45° 0° (ie., front) 45° 90° respectively
 */
void scanWorker(int *obstacles);

/**
 * Initializes the deadReckoning, call it once \
 * 	before calling deadRWorker
 */
void deadRWorkerInit();

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
void deadRWorker(double prevX, double prevY, double prevA, double *X, double *Y, double* A);

/*
 * Does one iteration going to (x,y) in a straight line
 * \param[in] x,y,a current position and heading of the robot, a in radian
 * \param[in] targetX,targetY coordinates of the target
 * \parma[in] power maximum power used
 * \return the error in cm between (x,y) and (targetX,targetY)
 */
double deadreckoningGoTo(const double x, const double y, const double a, const double targetX, const double targetY, int power);
#endif /* WORKERS_H_ */
