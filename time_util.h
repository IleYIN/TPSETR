/*
 * time_util.h
 *
 *  Created on: 16 janv. 2017
 *      Author: grolleau
 */

#ifndef TIME_UTIL_H_
#define TIME_UTIL_H_
#include <time.h>

/*
 * Adds ms milliseconds to the timespec t
 */
void add_ms(struct timespec *t, long ms);

#endif /* TIME_UTIL_H_ */
