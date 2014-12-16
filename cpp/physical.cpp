/*
	Physical Layer emulation using TCP
	
	It has only a few functions.
		Open Connection
		Close Connection
		Send (without confirmation)
		Receive (without confirmation)
		
	Error checking and packet confirmations are left to other layers
*/

#include <sys/socket.h>    //socket
#include <arpa/inet.h>
#include <netinet/tcp.h>
#include <netdb.h> /* hostent */
#include <errno.h>

#include <iostream>
#include <stdexcept>

#include "physical.h"
//Author: Akshay Thejaswi
Physical::Physical():
	is_client(false)
{
	//std::cout << "Physical constructor()" << std::endl;
			
	m_sockfd = socket(PF_INET , SOCK_STREAM , IPPROTO_TCP);

	if (m_sockfd == -1)
	{
		throw std::runtime_error("create socket failed"); //error creating socket
	}

			
}

//Author: Akshay Thejaswi
void Physical::Connect(char* serverurl, uint16_t portnum, uint8_t clientid){

	struct sockaddr_in serveraddr;
	memset(&serveraddr, 0, sizeof(serveraddr));
	
	struct hostent *server;
	server = gethostbyname(serverurl);
	if (server == NULL)
	{
		throw std::runtime_error("error resolving host or host does not exist."); //error getting host by name
	}
	bzero((char *) &serveraddr, sizeof(serveraddr)); 
	serveraddr.sin_family = AF_INET;
	bcopy((char *)server->h_addr,
		  (char *)&serveraddr.sin_addr.s_addr,
		  server->h_length);
	serveraddr.sin_port = htons(portnum);
	if (connect(m_sockfd , (struct sockaddr *)&serveraddr , sizeof(serveraddr)) < 0)
	{
		throw std::runtime_error("connect to webserver failed."); //error connecting to webserver
	}
	
	is_client = true; //only a client would use Connect
	m_clientid = clientid;
}

//Author: Akshay Thejaswi
int Physical::Bind(uint16_t portnum){
	struct sockaddr_in sockaddr;
	memset(&sockaddr, 0, sizeof(sockaddr));
	
	sockaddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Any incoming interface */
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_port = htons( portnum );
	int ret = bind(m_sockfd, (struct sockaddr *) &sockaddr, sizeof(sockaddr));
	if (ret < 0){
		throw std::runtime_error("error binding to socket");
	}
}

//Author: Akshay Thejaswi
int Physical::Listen(uint8_t maxpending){
	//listen for connecting clients
	int ret = listen(m_sockfd, maxpending);
	if (ret < 0){
		throw std::runtime_error("error listening on socket");
	}
	return ret;

}

//Author: Akshay Thejaswi
bool Physical::Accept(){

	//handle a connecting client in a child process
	struct sockaddr_in clientaddr;
	socklen_t clientlen = sizeof(clientaddr);
	int client_sock = accept(m_sockfd, (struct sockaddr *) &clientaddr, &clientlen);
	if (client_sock < 0){
		throw std::runtime_error("error accepting client");
	}
	
	pid_t pid = fork();
	if (pid < 0) {
		throw std::runtime_error("error forking application");
	}
	else if (pid == 0) {
		//update socket information for child process
		m_sockfd = client_sock;

	}
	
	return (pid == 0); //return true if current process is a child
}

//Author: Akshay Thejaswi
int Physical::Send(const char* msgbuf, size_t msglen, int flags){
	int ret = send(m_sockfd, msgbuf, msglen, flags);
	if (ret < 0) {
		throw std::runtime_error("error sending message");
	}
	return ret;
}


//Author: Akshay Thejaswi
int Physical::Receive(char* msgbuf, size_t msglen, int flags, struct timeval* timeout){

	//select instead of poll
	fd_set m_readfds;
	FD_ZERO(&m_readfds);
	FD_SET(m_sockfd, &m_readfds);

	int sockchange = select(m_sockfd+1, &m_readfds , NULL , NULL , timeout);
	if ((sockchange < 0) && (errno!=EINTR)){ //error using select
		//std::cout << errno << std::endl;
		throw std::runtime_error("select error");
	}

	if (FD_ISSET(m_sockfd, &m_readfds)){ //any changes?
		int mcount;
		
		//read in data and check if disconnected
		if ((mcount = recv(m_sockfd, msgbuf, msglen, flags)) == 0){ 
			Close();
		}

		return mcount;
	}
	else {
		return -1;
	}
}

//Author: Akshay Thejaswi
void Physical::Close(){
	LOG << "Disconnected" << std::endl;
	close(m_sockfd);
	throw std::runtime_error("Partner disconnected.");
	//throw std::runtime_error("Partner disconnected.");
}
