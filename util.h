#include <stdio.h>
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 14565

unsigned long resolveName(char* name);
void dieWithError(char* message);

