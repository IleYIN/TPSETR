/*
 * communication.h
 *
 *  Created on: 25 janv. 2017
 *      Author: grolleau
 */

#ifndef COMMUNICATION_H_
#define COMMUNICATION_H_

/**
 * Creates a TCP server and waits for ground station connection
 * \param outStream[out] output stream to the ground station (you can use fprintf to send to the ground station)
 * \param inStream[out] input stream from the client (you can use fscanf to read from the ground station)
 */
int WaitClient(FILE **outStream,FILE ** inStream);

void CloseSockets();

#endif /* COMMUNICATION_H_ */
