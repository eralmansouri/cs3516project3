#include <exception>
#include "server.h"
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
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
				std::cout << re.what() << std::endl;
			}
			break;
		}
	}
}

void Server::HandleClient(){
	
	char filename[15];
	Receive((char*)&m_clientid, sizeof(m_clientid)); //set m_clientid to the connected client id
	sprintf(filename, "server_%d.log", m_clientid);
	outlogfile.open(filename, std::ofstream::out | std::ofstream::app);
	LOG << "Connected" << std::endl;
	
	uint8_t numofphotos;
	Receive((char*)&numofphotos, sizeof(numofphotos));
	//std::cout << "Client (" << getClientId()*1 << ") has connected." << std::endl;
	char picbuffer[201];
	
	
	for (uint8_t i = 1; i <= numofphotos; i++){
		
		sprintf(filename, "photonew%d%d.jpg", m_clientid, i);
		std::ofstream photofile (filename, std::ofstream::out | std::ofstream::binary);
		if(!photofile.is_open()) throw std::runtime_error("error opening output image file.");
		int writebytes, pic_eof;
		
		do {
			writebytes = Receive(picbuffer, sizeof(picbuffer));
			pic_eof = picbuffer[0];
			//std::cout << "writebytes: " << writebytes << std::endl;
			photofile.write(&picbuffer[1], writebytes-1);
		} while (pic_eof == 0); //0 means it's not the final message!
		
		//for (int i = 0; i < 201; i++) std::cout << picbuffer[i]*1;
		photofile.close();
	}
	//std::cout << "Closing photofile." << std::endl;
	//std::cout << echostr << std::endl;
	LOG << "Client finished sending pictures" << std::endl;
	outlogfile.close();
}

int main(int argc , char *argv[]){
	Server start_server(7171);
	return 0;
}
