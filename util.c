#include "util.h"
#include "stdlib.h"

//gets the address of the given host
unsigned long resolveName(char* name)
{
	struct hostent *host;

	 if((host = gethostbyname(name)) == NULL){
	 	dieWithError("gethostbyname() failed");
	 }

	 return *((unsigned long *)host->h_addr_list[0]);
}

//prints an error and exits the program
void dieWithError(char* message)
{
	fprintf(stderr, "%s\n", message);
	exit(1);
}
