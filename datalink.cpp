#include <iostream>
#include "datalink.h"

DataLink::DataLink(std::string serverurl, uint16_t portnum)
	:Physical(serverurl, portnum)
{
	std::cout << "DataLink constructor()" << std::endl;
}

int DataLink::Send(char* msgbuf, size_t msglen)
{
	char frame[130];
	frame[0] = 0x01; //data payload
	frame[1] = msglen;
	//frame[2] = bytesequence; //2 bytes
	//frame[4] = error detection; //4->5 2 bytes
	
	memcpy(&frame[6], msgbuf, msglen);
	std::cout << "DataLink::Send called" << std::endl;
	Physical::Send(frame, sizeof(frame));
}

int DataLink::ReadBytes(char* msgbuf, size_t msglen)
{
	//read X number of bytes before returning.


}

int DataLink::Receive(char* msgbuf, size_t msglen)
{
	/*
		TODO: Read frame info, error check, send ACK
		
		1 byte frame type
		2 bytes sequence
		1 byte end of frame
		2 bytes error detection
		
		
		1->124 bytes payload
	*/
	char frame[256];
	uint8_t bytesleft;
	int bcount = Physical::Receive(frame, sizeof(frame));
	if(bcount > 0){
		uint8_t frametype = frame[0];
		if(frametype == 0x00){ //ACK
			//
		
		}
		else if(frametype == 0x01){ //Data
		
		}
	
	
	}
	return bcount;

}
