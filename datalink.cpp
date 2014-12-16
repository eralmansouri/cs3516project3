#include <iostream>
#include <stdexcept>
#include "datalink.h"

#define MAX_FRAME_SIZE 130 // 24 //up to 255?
#define MAX_PAYLOAD_SIZE 124

DataLink::DataLink():
	m_writesequence(1),
	m_readsequence(1)
{
	//logging purposes only
	m_failedack = 0;
	m_pktsent = 0; 
	m_duplicate = 0;
	m_timeout = 0;
	m_errorack = 0;
	m_totalframesrecv = 0;
	m_framesreceived = 0;
	m_frameresent = 0;
	m_frameerror = 0;
	m_framesent = 0;
	m_acksent = 0;
}

//Inserts the given packet into frames and sends it to the physical layer
//Author: Essam Al-Mansouri
int DataLink::Send(const char* msgbuf, size_t msglen)
{
	m_pktsent++;

	//split any size packets into small frames
	m_framesthispacket = 1;
	size_t written = 0;
	do {
		//header for data frames
		FrameHeader fheader = {0x01, 0, 0};
		fheader.sequence = m_writesequence;
		char frame[MAX_FRAME_SIZE];

		
		//break into payloads of 124 bytes or less 
		uint8_t writelen = msglen - written;
		if(writelen > MAX_PAYLOAD_SIZE ) writelen = MAX_PAYLOAD_SIZE ;
		frame[sizeof(fheader)] = writelen; //frame payload size
		memcpy(&(frame[sizeof(fheader)+1]), (char*)&msgbuf[written], writelen);
		
		
		
		//create error detection bytes
		uint16_t checksum = 0;
		checksum ^= fheader.sequence;
		for (int i = 0; i < (writelen+1); i++){
			checksum ^= frame[sizeof(struct FrameHeader)+i];
		}
		fheader.errorcheck = checksum;
		
		//simulated error
		m_framesent++;
		if(is_client && ((m_framesent % 5) == 0)){
			fheader.errorcheck ^= 1; //toggle the last bit
		}
		
		//create frame buffer to pass to physical layer
		memcpy(&frame, &fheader, sizeof(fheader));
		uint8_t framesize = writelen + sizeof(fheader) + 1;
		Physical::Send(frame, framesize);
		
		
		LOG << "Frame #" << m_framesthispacket*1 << " in packet #" << m_pktsent*1 << " successfully sent" << std::endl;
		
		//wait for acknowledgement package
		if (ReceiveFrameAck(m_writesequence)){
			written += writelen;
			
			m_framesthispacket++;
			m_writesequence++; //can be counter of success ACK!
		}
		else {
			LOG << "Retransmitting frame #" << m_framesthispacket*1 << " in packet #" << m_pktsent*1 << std::endl;
			m_failedack++;
			m_frameresent++;
		}
	} while (written < msglen);
	
	return written;
}

//Author: Akshay Thejaswi
bool DataLink::ReceiveFrameAck(uint16_t sequence){
	if(!is_client) return true; //servers dont need to wait for ack from client

	//read in the next ACK package from server
	
	FrameHeader ackframe;
	struct timeval tv = {0,250000}; //3 second timeout
	int bcount = 0;
	do {
		int bytesread = Physical::Receive(&((char*)&ackframe)[bcount], sizeof(ackframe)-bcount, 0, &tv);
		if(bytesread == -1){
			m_timeout++;
			LOG << "Timed out waiting for ACK (frame #" << m_framesthispacket*1 << " of packet #" << m_pktsent << ") " << std::endl;
			return false;
		}
		bcount += bytesread;
	} while(bcount < sizeof(ackframe));
	
	m_totalframesrecv++;
	
	//check for errors
	if (ackframe.sequence != ackframe.errorcheck){
		LOG << "Received Frame ACK (frame #" << m_framesthispacket*1 << " of packet #" << m_pktsent << ") with error" << std::endl;
		m_errorack++;
		return false;
	}
	m_framesreceived++;
	LOG << "Frame ACK (frame #" << m_framesthispacket*1 << " of packet #" << m_pktsent << ") received successfully." << std::endl;
	return true;
	
}

//Author: Essam Al-Mansouri
int DataLink::SendFrameAck(uint16_t sequence){
	if (is_client) return 0; //clients dont need to send ACK
	
	FrameHeader ackframe;
	ackframe.frametype = 0x00;
	ackframe.errorcheck = sequence; //error for ACK is just sequence number
	ackframe.sequence = sequence;
	
	m_acksent++;
	m_framesent++;
	if ((m_framesent % 13) == 0){
		ackframe.errorcheck ^= 1; //simulated error
	}
	return Physical::Send((char*)&ackframe, sizeof(ackframe));
}

//Author: Akshay Thejaswi
int DataLink::Receive(char* msgbuf)
{
	//read in at least 1 unique data frame
	
	FrameHeader frame;
	//read in frame header (type, error and sequence)
	ReceiveBytes((char*)&frame, sizeof(frame));

	if (frame.frametype == 0x01) { //payload package
		m_totalframesrecv++;
		uint8_t payloadsize; //frame size
		ReceiveBytes((char*)&payloadsize, sizeof(payloadsize));
		
		//read in the payload
		uint8_t bytesread = ReceiveBytes(msgbuf, payloadsize);
		
		//is it a legit payload? check error detection bytes, sequence, etc.
		uint16_t checksum = frame.sequence ^ 0;
		
		checksum ^= payloadsize;
		for (int i = 0; i < payloadsize; i++){
			checksum ^= msgbuf[i];
		}
	
		//does our checksum match the error detection bytes?
		if (checksum != frame.errorcheck){
			LOG << "Received frame " << m_readsequence*1 << " with error." << std::endl;
			m_frameerror++;
			return Receive(msgbuf); //try again in the next frame
		}
		
		
		//is this frame a duplicate?
		if (frame.sequence == m_readsequence){ //frame is expected, complete and correct
			SendFrameAck(frame.sequence);
			m_readsequence++;
			return bytesread;
		}
		else { //unexpected frame duplicate
			LOG << "Received a duplicate frame with sequence (" << frame.sequence*1 << "). Resending ack in case it was damaged" << std::endl;
			m_duplicate++;
			SendFrameAck(frame.sequence); //resend ACK for duplicate frames
			return Receive(msgbuf); //return the next frame
		}
		
		
	}
	else {
		return Receive(msgbuf);
	}
}

//Author: Essam Al-Mansouri
int DataLink::ReceiveBytes(char* msgbuf, size_t msglen)
{
	//std::cout << "DataLink::ReceiveBytes() called" << std::endl;
	uint8_t bcount = 0;
	do {
		bcount += Physical::Receive(&(msgbuf[bcount]), msglen - bcount);
	
	} while (bcount < msglen);
	return bcount;

}
