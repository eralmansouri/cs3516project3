#include <exception>
#include "server.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>

//Author: Essam Al-Mansouri
Server::Server(uint16_t portnum){
	Bind(7171);
	Listen();
	
	while (true){
		std::cout << "Server is now listening for clients." << std::endl;
		bool i_am_client = Accept();
		if (i_am_client){
			try {
				HandleClient();
			}
			catch (std::exception& re){
				//log statistics
				LOG << re.what() << std::endl;
				LOG << "Frames received: " << m_totalframesrecv*1 << std::endl;
				LOG << "Frames received in error: " << m_frameerror*1 << std::endl;
				LOG << "Duplicate frame received: " << m_duplicate*1 << std::endl;
				LOG << "Total ACK Frame sent: " << m_acksent*1 << std::endl;
				LOG << "Packet sent to Network Layer: " << m_pktreceived*1 << std::endl;
			}
			break; //clients don't continue the loop
		}
	}
}

//Author: Essam Al-Mansouri
void Server::HandleClient(){
	
	char filename[15];
	Receive((char*)&m_clientid, sizeof(m_clientid)); //receive our client id
	sprintf(filename, "server_%d.log", m_clientid);
	outlogfile.open(filename, std::ofstream::out);
	if (!outlogfile.is_open()) throw std::runtime_error("error opening server log file");
	LOG << "Connected" << std::endl;
	
	uint8_t numofphotos;
	
	//Receive the number of photos we will need to open
	Receive((char*)&numofphotos, sizeof(numofphotos));
	
	
	char picbuffer[201]; //200 byte buffer + 1 byte end-of-pic indicator 
	for (uint8_t i = 1; i <= numofphotos; i++){
		
		//open the appropriate file
		sprintf(filename, "photonew%d%d.jpg", m_clientid, i);
		std::ofstream photofile (filename, std::ofstream::out | std::ofstream::binary); //binary output
		
		if(!photofile.is_open()) throw std::runtime_error("error opening output image file.");
		
		int writebytes, pic_eof;
		do {
			writebytes = Receive(picbuffer, sizeof(picbuffer));
			pic_eof = picbuffer[0];
			photofile.write(&picbuffer[1], writebytes-1);
		} while (pic_eof == 0); //0 means it's not the final message!
		
		photofile.close();
	}

	LOG << "Client finished sending pictures" << std::endl;
	
	Receive((char*)picbuffer, sizeof(picbuffer)); 
	outlogfile.close();
}

//Author: Essam Al-Mansouri
int main(int argc , char *argv[]){
	Server start_server(7171);
	return 0;
}
