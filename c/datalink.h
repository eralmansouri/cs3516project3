#include "stdlib.h"
#include "util.h"
#include "physical.h"

struct datalink_frame_t {
	uint16_t sequence;
	uint8_t end;
	uint16_t checksum;
};

void datalink_send(char* buffer, int len, int sock);
int datalink_recieve(char* buffer, int max, int sock);
void datalink_recieve_ack(int sock);
void datalink_send_ack(int sock);
