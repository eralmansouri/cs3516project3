#include "datalink.h"

void datalink_send(char* buffer, int len, int sock)
{
	physical_send(buffer, len, sock);
}

void datalink_recieve(char* buffer, int len, int sock)
{
	physical_recieve(buffer, len, sock);
}