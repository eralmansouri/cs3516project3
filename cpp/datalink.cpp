#include <iostream>
#include <stdexcept>
#include "datalink.h"

#define FNV_OFFSET 
#define FNV_PRIME 0xb3
#define MAX_FRAME_SIZE 130 // 24 //up to 255?
#define MAX_PAYLOAD_SIZE 124
DataLink::DataLink():
	m_writesequence(0),
	m_readsequence(0)
{
	//logging purposes only
	m_failedack = 0;
	m_pktsent = 0; 
	m_duplicate = 0;
	m_timeout = 0;
	m_framesreceived = 0;
	m_frameresent = 0;
}

int DataLink::Send(const char* msgbuf, size_t msglen)
{
	m_pktsent++;
	//std::cout << "DataLink:Send() called" << std::endl;
	//split packets into frames
	uint8_t framesent = 0;
	size_t written = 0;
	do {
		FrameHeader fheader = {0x01, 0, 0};
		fheader.sequence = m_writesequence;
		char frame[MAX_FRAME_SIZE];
		
		//break into 124 byte payloads 
		uint8_t writelen = msglen - written;
		if(writelen > MAX_PAYLOAD_SIZE ) writelen = MAX_PAYLOAD_SIZE ;
		frame[sizeof(fheader)] = writelen; //frame payload size
		
		
		memcpy(&(frame[sizeof(fheader)+1]), (char*)&msgbuf[written], writelen);
		memcpy(&frame, &fheader, sizeof(fheader));
		uint8_t framesize = writelen + sizeof(fheader) + 1;
		framesent++;
		m_framesent++;
		Physical::Send(frame, framesize);
		
		//wait for acknowledgement package
		if (ReceiveFrameAck(m_writesequence)){
			written += writelen;
			m_writesequence++; //can be counter of success ACK!
		}
		else {
			LOG << "Resending frame #" << framesent*1 << " in packet #" << m_pktsent*1 << std::endl;
			m_failedack++;
			m_frameresent++;
		}
	} while (written < msglen);
	
	return written;
}

bool DataLink::ReceiveFrameAck(uint16_t sequence){
	//std::cout << "Waiting for ACK: " << sequence*1 << std::endl;
	FrameHeader ackframe;
	struct timeval tv = {3,0}; //3 second timeout
	int bcount = 0;
	do {
		int bytesread = Physical::Receive(&((char*)&ackframe)[bcount], sizeof(ackframe)-bcount, 0, &tv);
		if(bytesread == -1){
			m_timeout++;
			LOG << "Timed out waiting for ack frame for " << m_framesent*1 << std::endl;
			return false;
		}
		bcount += bytesread;
	} while(bcount < sizeof(ackframe));
	m_framesreceived++;
	//check for errors
	if (ackframe.frametype == 0x00 && ackframe.sequence == sequence && ackframe.sequence == ackframe.errorcheck){
		//std::cout << "Success! Ack received." << std::endl;

		return true;
	}
	else if (ackframe.frametype == 0x01){
		return ReceiveFrameAck(sequence);
	}

	return false;
}

int DataLink::SendFrameAck(uint16_t sequence){
	//std::cout << "Sending Frame ACK: " << sequence*1 << std::endl;
	FrameHeader ackframe;
	ackframe.frametype = 0x00;
	ackframe.errorcheck = sequence;
	ackframe.sequence = sequence;
	
	//if(m_readsequence == sequence) m_readsequence++;
	LOG << "Sending an ACK frame" << std::endl;
	m_framesent++;
	return Physical::Send((char*)&ackframe, sizeof(ackframe));
}


//purpose is to read at least 1 frame into msgbuf
int DataLink::Receive(char* msgbuf)
{
	//std::cout << "DataLink:Receive() called" << std::endl;
	FrameHeader frame;

	ReceiveBytes((char*)&frame, sizeof(frame));

	if (frame.frametype == 0x00) { //ack package
		//is it a legit ack? check error detection bytes, sequence and then
		//delete item from frame buffer
		//uhmm... weird? we were expecting an ack frame.
		LOG << "Received an unexpected ACK frame!" << std::endl;
		return Receive(msgbuf);
	}
	else if (frame.frametype == 0x01) { //payload package
	
		uint8_t payloadsize; //end of frame indicator
		ReceiveBytes((char*)&payloadsize, sizeof(payloadsize));
		
		//std::cout << "Frame: " << frame.frametype*1 << ", Payload Size: " << payloadsize*1 << std::endl;
		//uint8_t payload[MAX_FRAME_SIZE - sizeof(frame) - sizeof(payloadsize)]; 
		uint8_t bytesread = ReceiveBytes(msgbuf, payloadsize);
		
		//is it a legit payload? check error detection bytes, sequence and then trigger network/application receive

		
		
		
		
		
		if (frame.sequence == m_readsequence){ //frame is expected, complete and correct
			SendFrameAck(frame.sequence);
			m_readsequence++;
			return bytesread;
		}
		else { //unexpected frame sequence
			LOG << "Received a frame with unexpected sequence (" << frame.sequence*1 << "). Resending ack in case it is damaged" << std::endl;
			m_duplicate++;
			SendFrameAck(frame.sequence);
			return Receive(msgbuf);
		}
		
		
	}
	else {
		throw std::runtime_error("unrecognized frame header");
	}
}

int DataLink::ReceiveBytes(char* msgbuf, size_t msglen)
{
	//std::cout << "DataLink::ReceiveBytes() called" << std::endl;
	uint8_t bcount = 0;
	do {
		bcount += Physical::Receive(&(msgbuf[bcount]), msglen - bcount);
	
	} while (bcount < msglen);
	return bcount;

}
