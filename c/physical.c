#include "physical.h"

int physical_connect(char* serverurl, int portnum)
{
	int m_sockfd;

	struct sockaddr_in serveraddr;
	struct hostent *server;
	m_sockfd = socket(PF_INET , SOCK_STREAM , IPPROTO_TCP);
	if (m_sockfd == -1)
	{
		dieWithError("Error creating socket") ;//error creating socket
	}
	server = gethostbyname(serverurl);
	if (server == NULL)
	{
		dieWithError("error resolving host or host does not exist."); //error getting host by name
	}
	bzero((char *) &serveraddr, sizeof(serveraddr)); 
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		  (char *)&serveraddr.sin_addr.s_addr,
		  server->h_length);
	serveraddr.sin_port = htons(portnum);
	if (connect(m_sockfd , (struct sockaddr *)&serveraddr , sizeof(serveraddr)) < 0)
	{
		dieWithError("connect to webserver failed."); //error connecting to webserver
	}

	return m_sockfd;
}

void physical_send(char* buffer, int len, int socket)
{

}

void physical_recieve(char* buffer, int len, int socket)
{

}

void physical_close(int socket)
{

}

void physical_bind(int port)
{

}

int physical_accept(int socket)
{

}