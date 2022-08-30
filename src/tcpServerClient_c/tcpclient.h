#ifndef TCPCLIENT_H
#define TCPCLIENT_H


#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#include <unistd.h>

#define PORT 8888


unsigned char *gen_bytestream (size_t num_bytes);
/*
 * 
 * Generate byte stream composed of alphanumerics or simply letter
 * and allocate the desired number of bytes "num_bytes" in memory
 * to be sent out
 *
*/


int startTCPclient();
/*
 * Initiate the client side of a TCP connection 
 */

#endif 


void sendBytes(int num_bytes);
/*
 * Send payload with "num_bytes" size
 */

