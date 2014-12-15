#include <stdio.h>
#include <stdlib.h>
#include "util.h"

void sendPhoto(char* fileName, int sock);

int main(int argc, char** argv)
{
	int sock;
	char* serverAddress;
	int id;
	int numPhotos;

	if(argc != 4){
		fprintf(stderr, "Usage: %s server id num_photos\n", argv[0]);
		exit(1);
	}

	id = atoi(argv[2]);
	numPhotos = atoi(argv[3]);

	sock = physical_connect(argv[1]);

	int photo;
	for(photo = 0; photo < numPhotos; photo++){
		char photoName[1024];
		sprintf(photoName, "photo%d%d.jpg", id, photo);
		sendPhoto(photoName, sock);
	}

	physical_close(sock);

	return 0;
}

void sendPhoto(char* fileName, int sock){

	FILE *fp;
	fp = fopen(fileName, "r");

	char chunk[200];
	int chunkSize = 0;
	int total = 0;
	while((chunkSize = fread(chunk, 1, 200, fp)) > 0){
		total += chunkSize;
		datalink_send(chunk, chunkSize, sock);
	}
	printf("%s was %d bytes\n", fileName, total);

}