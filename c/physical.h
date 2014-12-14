#include <netdb.h>
#include <errno.h>
#include "util.h"

int physical_connect(char* serverurl, int portnum);

void physical_send(char* buffer, int len, int socket);

void physical_recieve(char* buffer, int len, int socket);

void physical_close(int socket);
void physical_bind(int port);

int physical_accept();