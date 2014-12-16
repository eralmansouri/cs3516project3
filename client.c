#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include <string.h>
#include "log.h"

void sendFileCount(int n, int sock);
void sendPhoto(char* fileName, int sock);

int packetCounter = 0;

//Akshay
// Main method for photo client
int main(int argc, char** argv)
{
	int sock;
	char* serverAddress;
	int id;
	int numPhotos;

	if(argc != 4){ //check arguments
		fprintf(stderr, "Usage: %s server id num_photos\n", argv[0]);
		exit(1);
	}

	//get arguments
	id = atoi(argv[2]);
	numPhotos = atoi(argv[3]);

	//start the logging
	char logFileName[1024];
	sprintf(logFileName, "client_%d.log", id);
	log_init(logFileName);

	sock = physical_connect(argv[1]);
	sendFileCount(numPhotos, sock); //send the server the number of photos to expect
	int photo;
	//send each photo
	for(photo = 0; photo < numPhotos; photo++){
		char photoName[1024];
		sprintf(photoName, "photo%d%d.jpg", id, photo);
		sendPhoto(photoName, sock); 
	}

	//close the connection
	physical_close(sock);
	log_finish();

	return 0;
}

//Akshay
//send a single integer to the server
void sendFileCount(int n, int sock){
	datalink_send((char*)&n, sizeof(int), sock);
}

//Akshay
//send a photo to the server from the given file name
void sendPhoto(char* fileName, int sock){

	FILE *fp;
	fp = fopen(fileName, "r");
	if(!fp){
		printf("file null\n");
	}
	char chunk[MAX_CHUNK_SIZE];
	int chunkSize = 0;
	int total = 0;
	int pcount = 0;
	//read the file in 200 byte chunks
	while((chunkSize = fread(chunk, 1, MAX_CHUNK_SIZE, fp)) > 0){
		total += chunkSize;

		char packet[chunkSize + 1];
		packet[0] = feof(fp);

		memcpy(&packet[1], chunk, chunkSize);
		if(packet[0]){
			printf("eof after %d bytes\n", total);
		}
		packetCounter++;
		//send the chunk to the datalink layer
		datalink_send(packet, chunkSize+1, sock);
		fprintf("packet %d sent\n", packetCounter);
		//wait for network level ACK
		datalink_recieve_ack(sock);
		fprintf("ACK packet %d recieved successfully\n", packetCounter);
		pcount++;
	}
	printf("%s was %d bytes, over %d packets\n", fileName, total, pcount);

}