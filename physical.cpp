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

Physical::Physical(std::string serverurl, uint16_t portnum){
	std::cout << "Physical constructor()" << std::endl;
	struct sockaddr_in serveraddr;
	struct hostent *server;
	m_sockfd = socket(PF_INET , SOCK_STREAM , IPPROTO_TCP);
	if (m_sockfd == -1)
	{
		throw std::runtime_error("create socket failed"); //error creating socket
	}
	server = gethostbyname(serverurl.c_str());
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
	
}

int Physical::Send(char* msgbuf, size_t msglen){
	//check if disconnected
	std::cout << "Physical::Send called" << std::endl;
	return send(m_sockfd, msgbuf, msglen, 0);
}


//TODO: Callback function instead of Receive();
int Physical::Receive(char* msgbuf, size_t msglen){

	FD_ZERO(&m_readfds);
	FD_SET(m_sockfd, &m_readfds);
	
	//select instead of poll
	int sockchange = select(m_sockfd, &m_readfds , NULL , NULL , NULL);
	if ((sockchange < 0) && (errno!=EINTR)){
		throw std::runtime_error("select error");
	}
	else if(sockchange == 0){
		return -1;
	}
	int mcount;
	
	//check if disconnected
	if ((mcount = read(m_sockfd, msgbuf, msglen)) == 0){ //read instead of recv?
		Close();
	}
	
	return mcount;
}

void Physical::Close(){
	close(m_sockfd);
}
