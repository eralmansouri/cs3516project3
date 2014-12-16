/*
	Physical Layer emulation using TCP
	
	It has only a few functions.
		Open Connection
		Close Connection
		Send (without confirmation)
		Receive (without confirmation)
		
	Error checking and packet confirmations are left to other layers
*/

#define MAX_MSG_SIZE 254
#include "datalink.h"
class Network : public DataLink {
	public:
	Network();
	void Send(const char*, uint8_t);
	int Receive(char*, uint8_t);
	
	uint32_t m_pktreceived;
	private:
	struct Message {	
		uint8_t msgsize;
		char msgbuff[MAX_MSG_SIZE];
	} msg;
	uint8_t m_buffered;
	uint8_t m_read;
	
	uint16_t m_pktwriteseq;
	uint16_t m_pktreadseq;
	
	
	uint32_t m_ackpktsent;
	uint32_t m_ackpktrecv;
};
