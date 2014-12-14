#include "physical.h"

int physical_connect(char* serverAddress)
{
	int sock;

	struct sockaddr_in serverAddr;
	struct hostent *server;

	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		dieWithError("socket() failed");
	}

	memset(&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_addr.s_addr = resolveName(serverAddress);
	serverAddr.sin_port = htons(PORT);

	if(connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		dieWithError("connect() failed");
	}

	return sock;
}

void physical_send(char* buffer, int len, int socket)
{

}

void physical_recieve(char* buffer, int len, int socket)
{

}

void physical_close(int sock)
{
	close(sock);
}

int physical_bind()
{
	int sock;

	struct sockaddr_in serverAddr;
	
	if((sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0){
		dieWithError("socket() failed");
	}

	//construct local address structure
	memset(&serverAddr, 0, sizeof(serverAddr));		//zero out structure
	serverAddr.sin_family = AF_INET;				//internet address family
	serverAddr.sin_addr.s_addr = htonl(INADDR_ANY); //any incoming interface
	serverAddr.sin_port = htons(PORT);	//local port

	if(bind(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		dieWithError("bind() failed");
	}

	if(listen(sock, MAXPENDING) < 0){
		dieWithError("listen() failed");
	}

	return sock;
}

int physical_accept(int socket)
{
	int clientSocket;
	struct sockaddr_in clientAddr;
	int clientLen = sizeof(clientAddr);
	if((clientSocket = accept(socket, (struct sockaddr*)&clientAddr, clientLen)) < 0){
		dieWithError("accept() failed");
	}

	return clientSocket;
}