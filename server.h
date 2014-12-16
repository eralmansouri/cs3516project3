/*
	Physical Layer emulation using TCP
	
	It has only a few functions.
		Open Connection
		Close Connection
		Send (without confirmation)
		Receive (without confirmation)
		
	Error checking and packet confirmations are left to other layers
*/
#include "network.h"

class Server : public Network {
	public:
	Server(uint16_t); //start server
	void HandleClient();
	
	private:

};
