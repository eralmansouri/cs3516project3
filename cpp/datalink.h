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
	DataLink();
	int Send(const char*, size_t);
	int Receive(char*);
	int ReceiveBytes(char*, size_t);
	bool ReceiveFrameAck(uint16_t);
	int SendFrameAck(uint16_t);
	private:
	struct FrameHeader {
		uint8_t frametype;
		uint16_t sequence;
		uint16_t errorcheck;
	};
	int SendPayload(const char*, size_t);
	int SendAck();
	uint16_t m_writesequence;
	uint16_t m_readsequence;
	
	
	//logging purposes only
	uint32_t m_failedack;
	uint32_t m_pktsent;
	uint32_t m_timeout;
	uint32_t m_framesent;
	uint32_t m_framesreceived;
	uint32_t m_frameresent;
	uint32_t m_duplicate;
};
