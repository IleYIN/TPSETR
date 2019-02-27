/*
 * communication.c
 *
 *  Created on: 25 janv. 2017
 *      Author: grolleau
 */
#include<stdio.h>
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write

#define PORT 2224
static int socket_desc;


int WaitClient(FILE **outStream,FILE ** inStream) {
	int client_sock,client_sock2;
    struct sockaddr_in server , client;
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1) {
        perror("Could not create socket. Error");
        return 1;
    }
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0) {
        //print the error message
        perror("bind failed, you should wait a bit for the port to be freed by the system. Error");
        return 1;
    }
    listen(socket_desc , 1);
    int s = sizeof(struct sockaddr_in);
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&s);
    if (client_sock < 0) {
        perror("accept failed");
        return 1;
    }
    client_sock2=dup(client_sock);
    *inStream=fdopen(client_sock,"r");
    *outStream=fdopen(client_sock2,"w");
    if (*inStream==0 || *outStream==0) {
    	return 1;
    }
    return 0;
}

void CloseSockets() {
	close(socket_desc);
}
