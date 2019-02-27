/*
 * mdd.c
 *
 *  Created on: 25 janv. 2017
 *      Author: grolleau
 */
#include <pthread.h>
#include <string.h>
#include <malloc.h>
#include "mdd.h"

/**
 * Initializes a shared int
 *\param val[in] initial value of the shared data
 *\return the new shared int, memory allocation is done inside the function
 */
MDD_int MDD_int_init(const int val) {
	MDD_int m=(MDD_int)malloc(sizeof(struct s_MDD_int));
	m->val=val;
	m->dirty=0;
	pthread_mutex_init(&(m->mutex),0);
	return m;
}


MDD_gen MDD_gen_init(const int size) {
	MDD_gen m=(MDD_gen)malloc(sizeof(struct s_MDD_gen));
	m->buff=malloc(size);
	m->size=size;
	m->dirty=0;
	pthread_mutex_init(&(m->mutex),0);
	return m;
}


/**
 * Writes in a shared int
 *\param mdd[in,out] the shared data
 *\param val[in] value to write in the shared data
 */
void MDD_int_write(MDD_int mdd, const int val) {
	pthread_mutex_lock(&(mdd->mutex));
	mdd->val=val;
	mdd->dirty=1;
	pthread_mutex_unlock(&(mdd->mutex));
}

void MDD_gen_write(MDD_gen mddg, const char * buff){
	pthread_mutex_lock(&(mddg->mutex));
	memcpy(mddg->buff,buff,mddg->size);
	mddg->dirty=1;
	pthread_mutex_unlock(&(mddg->mutex));

}



/**
 * Reads in a shared int
 *\param mdd[in] the shared data
 *\return value of the shared data
 */
int MDD_int_read(MDD_int mdd) {
	int val;
	pthread_mutex_lock(&(mdd->mutex));
	val=mdd->val;
	mdd->dirty=0;
	pthread_mutex_unlock(&(mdd->mutex));
	return val;
}


char *  MDD_gen_read(MDD_gen mddg){
	char * buff;
	pthread_mutex_lock(&(mddg->mutex));
	memcpy(buff,mddg->buff,mddg->size);
	mddg->dirty=0;
	pthread_mutex_unlock(&(mddg->mutex));
	return buff;
}




/**
 * Reads in a shared int, while returning if it was dirty
 *\param mdd[in] the shared data
 *\param val[out] value of the shared data
 *\return  1 if the shared data was modified since last reading (i.e., dirty), 0 else
 */
int MDD_int_read2(MDD_int mdd,int *val) {
	int dirty;
	pthread_mutex_lock(&(mdd->mutex));
	*val=mdd->val;
	dirty=mdd->dirty;
	mdd->dirty=0;
	pthread_mutex_unlock(&(mdd->mutex));
	return dirty;
}

int MDD_gen_read2(MDD_gen mddg, char ** buff){//*buff est pour retourner le contenant adresse de mddg
	int dirty;
	pthread_mutex_lock(&(mddg->mutex));
	memcpy(**buff,mddg->buff,mddg->size);
	dirty=mddg->dirty;
	mddg->dirty=0;
	pthread_mutex_unlock(&(mddg->mutex));
	return dirty;
}

