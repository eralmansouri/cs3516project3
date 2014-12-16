#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 14565
#define MAX_CHUNK_SIZE 200
#define MAX_PACKET_SIZE 201

unsigned long resolveName(char* name);
void dieWithError(char* message);

