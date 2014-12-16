#include <iostream>
#include <stdexcept>

#include "network.h"

//Author: Akshay Thejaswi
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

//Author: Akshay Thejaswi
void Network::Send(const char* buffer, uint8_t length){
	//create a packet of variable length and send it using DataLink layer
	char msgbuff[MAX_MSG_SIZE];
	
	memcpy ( &msgbuff[0], &length, sizeof(length) );
	memcpy ( &msgbuff[ sizeof(length)], buffer, length );

	DataLink::Send(msgbuff, length + sizeof(length));
	LOG << "Packet #" << m_pktsent*1 << " successfully sent." << std::endl;
}

//Author: Akshay Thejaswi
int Network::Receive(char* buffer, uint8_t length){
	//read in at least one full packet from datalink layer by combining frames
	
	if (!(m_read < m_buffered)){
		//do we already have bytes buffered?
		m_read = 0;
		m_buffered = DataLink::Receive((char*)&msg) - sizeof(msg.msgsize);
		while (m_buffered < msg.msgsize) {
			m_buffered += DataLink::Receive((char*)&msg.msgbuff[m_buffered]);
		}
		m_pktreceived++;
	}
	
	uint8_t read_bytes = m_buffered - m_read;
	
	//return rest of the packet or up to length
	if (read_bytes > length){
		read_bytes = length;
	}
	
	memcpy(buffer, (char*)&msg.msgbuff[m_read], read_bytes);
	m_read += read_bytes;

	return read_bytes; //return the number of bytes we copied into the buffer
}
