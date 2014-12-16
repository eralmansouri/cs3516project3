#include "physical.h"

class DataLink : public Physical {
	public:
	DataLink();
	int Send(const char*, size_t);
	int Receive(char*);
	int ReceiveBytes(char*, size_t);
	bool ReceiveFrameAck(uint16_t);
	int SendFrameAck(uint16_t);
	
	
	
	//logging purposes only
	uint32_t m_acksent;
	uint32_t m_failedack; //failed to receive ack
	uint32_t m_errorack; //ack with error
	uint32_t m_pktsent;
	uint8_t m_framesthispacket;
	uint32_t m_timeout;
	uint32_t m_framesent;
	uint32_t m_framesreceived;
	uint32_t m_frameerror;
	uint32_t m_frameresent;
	uint32_t m_duplicate;
	uint32_t m_totalframesrecv;
	
	private:
	struct FrameHeader {
		uint8_t frametype;
		uint16_t sequence;
		uint16_t errorcheck;
	};
	uint16_t m_writesequence;
	uint16_t m_readsequence;
	
	

};
