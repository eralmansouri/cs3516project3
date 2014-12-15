#include <netdb.h>
#include <errno.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <netinet/tcp.h>
#include <string.h>
#include "util.h"

#define MAXPENDING 5

int physical_connect(char* serverurl);

void physical_send(char* buffer, int len, int socket);
void physical_recieve(char* buffer, int len, int socket);

int physical_bind();
int physical_accept();

void physical_close(int sock);
