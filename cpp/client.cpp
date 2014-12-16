#include "client.h"
#include <fstream>
#include <stdexcept>

#include <iostream>
#include <string>
Client::Client(char* serverurl, uint8_t client_id, uint8_t numofphotos)
	: m_id(client_id)
{
	if (client_id > 9) throw std::runtime_error("only 1 digit client_id is allowed.");
	Connect(serverurl, 7171, client_id); 
	
	char filename[15];
	sprintf(filename, "client_%d.log", m_clientid);
	outlogfile.open(filename, std::ofstream::out | std::ofstream::app);
	
	Send((char*)&client_id, sizeof(client_id));
	Send((char*)&numofphotos, sizeof(numofphotos));
	
	for (uint8_t i = 1; i <= numofphotos; i++){
		sprintf(filename, "photo%d%d.jpg", client_id, i);
		SendPhoto(filename);
	}
	
	Close();
	outlogfile.close();
	//print out statistics here

}

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

int main(int argc , char *argv[]){
	if (argc < 4) {
		std::cerr << "Usage: " << argv[0] << " ServerMachine ID NumOfPhotos" << std::endl;
		return 1;
	}
	//std::string serverurl(argv[1]);
	Client photoclient(argv[1], atoi(argv[2]), atoi(argv[3]));


	return 0;
}
