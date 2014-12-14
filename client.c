#include <stdio.h>
#include <stdlib.h>
#include "util.h"

int main(int argc, char** argv)
{
	int sock;
	char* serverAddress;
	char* id;
	char* numPhotos;

	if(argc != 4){
		fprintf(stderr, "Usage: %s server id num_photos\n", argv[0]);
		exit(1);
	}

	sock = physical_connect(argv[1]);

	return 0;
}