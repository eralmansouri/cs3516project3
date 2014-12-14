/*
	Datalink Layer emulation using TCP
	
	It's purpose is to act as a "network protocol" that builds on the hardware layer
	
	In addition to utilizing hardware layer functions (open, close, send, recv):
		Error checking
		Arranging packets
		Handling ACKs
		
	Using the frame header, we can check for errors and respond to ACK, or propogate packet up to network layer.
	Header parameters: Checksum? Header Length and Type
	
	DataLink adds extra properties to send() using timeout and acknowledgement
*/

#include "physical.h"

class DataLink : public Physical {
	public:
	DataLink(std::string, uint16_t);
	int Send(char*, size_t);
	int Receive(char*, size_t);
	
	int ReadBytes(char*, size_t);
	private:
	uint16_t m_sequence;
};
