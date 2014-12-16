#include <iostream>
#include <stdexcept>

#include "network.h"

Network::Network():
	m_buffered(0),
	m_read(0)
{
	m_pktwriteseq = 0;
	m_pktreadseq = 0;
	m_pktreceived = 0;
	m_ackpktsent = 0;
	m_ackpktrecv = 0;
}

void Network::Send(const char* buffer, uint8_t length){
	//std::cout << "Network::Send() called" << std::endl;
	char msgbuff[MAX_MSG_SIZE];
	memcpy ( &msgbuff[0], &length, sizeof(length) );
	memcpy ( &msgbuff[ sizeof(length)], buffer, length );
	//std::cout << "Header Size: " << (length)*1 << ". String Length: " << length*1 << std::endl;
	DataLink::Send(msgbuff, length + sizeof(length));
}

int Network::Receive(char* buffer, uint8_t length){
	//std::cout << "Network::Receive() called" << std::endl;
	//do we already have data buffered?
	if (!(m_read < m_buffered)){
		//read 1 full packet
		m_read = 0;
		m_buffered = DataLink::Receive((char*)&msg) - sizeof(msg.msgsize);
		//
		while (m_buffered < msg.msgsize) {
			m_buffered += DataLink::Receive((char*)&msg.msgbuff[m_buffered]);
		}
		m_pktreceived++;
		//std::cout << "msg.msgsize: " << msg.msgsize*1 << ", m_buffered: " << m_buffered*1 << std::endl;
	}
	
	uint8_t read_bytes = m_buffered - m_read;
	
	//return rest of the packet or up to length
	if (read_bytes > length){
		read_bytes = length;
	}
	
	memcpy(buffer, (char*)&msg.msgbuff[m_read], read_bytes);
	m_read += read_bytes;
	//std::cout << "Read bytes: " << read_bytes*1 << std::endl;
	return read_bytes;
}
