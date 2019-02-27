/*
 * time_util.c
 *
 *  Created on: 16 janv. 2017
 *      Author: grolleau
 */
#include <time.h>

void add_ms(struct timespec *t, long ms) {
	t->tv_nsec+=ms%1000*1000000;
	t->tv_sec+=(ms/1000)+(t->tv_nsec/1000000000);
	t->tv_nsec%=1000000000;
}
