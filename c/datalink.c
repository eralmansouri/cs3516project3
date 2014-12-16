#include "datalink.h"

//sends a buffer to the physical layer with a frame
void datalink_send(char* buffer, int len, int sock)
{
	uint8_t frame[sizeof(struct datalink_frame_t) + len];
	struct datalink_frame_t* frame_header = (struct datalink_frame_t*)&frame;
	frame_header->sequence = 0;
	frame_header->end = 1;
	frame_header->checksum = 0;

	if(len > 0){
		memcpy(&frame[sizeof(struct datalink_frame_t)], buffer, len);
	}

	physical_send(frame, sizeof(frame), sock);
}

//recieves a frame from the physical layer and returns the packet
int datalink_recieve(char* buffer, int max, int sock)
{
	int frameSize = sizeof(struct datalink_frame_t);

	uint8_t frame[ frameSize + max];
	int size = physical_recieve(frame, max+frameSize, sock);

	memcpy(buffer, &frame[frameSize], size - frameSize);
	return size - frameSize;
}

//waits for an ack
void datalink_recieve_ack(int sock){
	char buf;
	if(datalink_recieve(&buf, 1, sock) > 0){
		dieWithError("Not Ack");
	}
}

//sends a network ack
void datalink_send_ack(int sock)
{
	datalink_send(NULL, 0, sock);
}