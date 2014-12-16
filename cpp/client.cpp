#include "client.h"
#include <fstream>
#include <stdexcept>

#include <iostream>
#include <string>
//Author: Essam Al-Mansouri
Client::Client(char* serverurl, uint8_t client_id, uint8_t numofphotos)
	: m_clientid(client_id)
{
	if (client_id > 9) throw std::runtime_error("only 1 digit client_id is allowed.");

	char filename[15];
	sprintf(filename, "client_%d.log", client_id);
	outlogfile.open(filename, std::ofstream::out);
	if (!outlogfile.is_open()) throw std::runtime_error("error opening client logfile.");
	
	try {
		Connect(serverurl, 7171, client_id); 
		


		Send((char*)&client_id, sizeof(client_id));
		Send((char*)&numofphotos, sizeof(numofphotos));
		
		for (uint8_t i = 1; i <= numofphotos; i++){
			sprintf(filename, "photo%d%d.jpg", client_id, i);
			SendPhoto(filename);
		}
		
		
		Close();
	}
	catch (std::exception& re){
		//print out statistics here

		LOG << "Number of frames sent: " << m_framesent*1 << std::endl;
		LOG << "Number of frame RE-transmission: " << m_frameresent*1 << std::endl;
		LOG << "Number of good ACKS: " << m_framesreceived*1 << std::endl;
		LOG << "Number of error ACKS: " << m_errorack*1 << std::endl;
		LOG << "Number of timeout ACKS: " << m_timeout*1 << std::endl;
	}
	
	outlogfile.close();

}

//Author: Essam Al-Mansouri
void Client::SendPhoto(char* filename){
	char buffer[201];
	std::ifstream photofile (filename, std::ifstream::in | std::ifstream::binary);
	if (!photofile.is_open()) throw std::runtime_error("error opening photo file.");
	
	//size_t filesize = 0;
	do {
		photofile.read(&buffer[1], 200);
		if(photofile) buffer[0] = 0;
		else buffer[0] = 1;
		Send(buffer, photofile.gcount() + 1);
	} while (photofile.good());
	photofile.close();
	
	//request ACK

	//m_network.Send(request.c_str(), request.size());
}

//Author: Essam Al-Mansouri
int main(int argc , char *argv[]){
	if (argc < 4) {
		std::cerr << "Usage: " << argv[0] << " ServerMachine ID NumOfPhotos" << std::endl;
		return 1;
	}
	//std::string serverurl(argv[1]);
	Client photoclient(argv[1], atoi(argv[2]), atoi(argv[3]));


	return 0;
}
