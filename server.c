#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "util.h"
#include <string.h>

void* clientHandler(void* args);
int recieveFile(int clientNum, int fileNum, int sock);

struct client_info_t {
	int sock;
	int num;
};

//main method for server
int main(int argc, char** argv)
{

	int count = 0;
	int sock = physical_bind();

	while(1){
		count++;		
		struct client_info_t* info = malloc(sizeof(struct client_info_t));
		pthread_t tid;
		info->sock = physical_accept(sock); //get the next client
		info->num = count;
		pthread_create(&tid, NULL, clientHandler, (void*)info); //start a new thread to handle the incoming client
	}

	return 0;
}

//handles a single client in a thread
void* clientHandler(void* args)
{
	//get the socket and client number
	struct client_info_t* info = (struct client_info_t*)args;
	int sock = info->sock;
	int clientNum = info->num;
	int fileCount = 0;
	int filesRecieved = 0;

	//the first packet is the number of files
	char fcBuf[4];
	datalink_recieve(fcBuf, sizeof(int), sock);
	memcpy(&fileCount, fcBuf, sizeof(int));

	//get each file
	for(filesRecieved = 0; filesRecieved < fileCount; filesRecieved++){
		int size = recieveFile(1, filesRecieved+1, sock);
		printf("recieved %d byte file\n", size);
	}

	physical_close(sock);

	return NULL;
}

//recieves and image from the client and stores it in photonew_[clientNum]_[fileNum].jpg
int recieveFile(int clientNum, int fileNum, int sock)
{
	FILE* fp;
	char fileName[1024];
	sprintf(fileName, "photonew_%d_%d.jpg", clientNum, fileNum);
	fp = fopen(fileName, "a");
	int finished = 0;
	int packetSize;
	char buf[MAX_PACKET_SIZE];
	int total = 0;
	int pcount = 0;
	//read packets until one has the EOF byte
	while(!finished){
		memset(buf, 0, MAX_PACKET_SIZE);
		packetSize = datalink_recieve(buf, MAX_PACKET_SIZE, sock);
		fwrite(&buf[1], 1, packetSize-1, fp); //write the packet to the file
		datalink_send_ack(sock);
		pcount++;
		total += packetSize-1;
		if(buf[0] == 1){
			finished = 1;
		}
	}
	fclose(fp);
	printf("%d packets\n", pcount);
	return total;
}
