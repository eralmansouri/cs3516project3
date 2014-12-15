#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv)
{
	int sock = physical_bind();

	int client = physical_accept(sock);

	physical_close(sock);
	return 0;
}