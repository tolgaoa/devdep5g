#include "tcpclient.h"

int sock = 0;

unsigned char* gen_bytestream (size_t num_bytes)
{
        unsigned char *stream = malloc (num_bytes);

        size_t i;
        for (i = 0; i < num_bytes; i++)
        {
                stream[i] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"[random () % 26];
        }

        return stream;
}


int msleep(unsigned int tms) {
  return usleep(tms * 1000);
}


void sendBytes(int num_bytes){

	size_t num = num_bytes;
	unsigned char* currBytes = gen_bytestream(num);
	//printf(currBytes);
	send(sock , currBytes , strlen(currBytes) , 0 );
	//printf("\nByte stream sent\n");
	msleep(100);
}


int startTCPclient()
{
	long valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		printf("\n Socket was not created \n");
		return -1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0){
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0){
		printf("\nConnection Failed \n");
		return -1;
	}

	send(sock , hello , strlen(hello) , 0 );
	printf("Hello message sent\n");
	//valread = read( sock , buffer, 1024);
	//printf("%s\n",buffer );
	//sleep(2);
}

