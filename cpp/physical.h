/*
	Physical Layer emulation using TCP
	
	It has only a few functions.
		Open Connection
		Close Connection
		Send (without confirmation)
		Receive (without confirmation)
		
	Error checking and packet confirmations are left to other layers
*/
#define LOG outlogfile << "Client (" << m_clientid*1 << "): " 
#include <fstream>
#include <string>
class Physical {
	public:
	Physical();
	
	void Connect(char*, uint16_t, uint8_t);
	int Bind(uint16_t);
	int Listen(uint8_t = 5); 
	bool Accept();
	int Send(const char*, size_t, int = 0);
	int Receive(char*, size_t, int = 0, struct timeval* = NULL);
	void Close();
	
	//logging related
	~Physical(){outlogfile.close();}
	
	uint8_t getClientId(){return m_clientid;}
	std::ofstream outlogfile;

	bool is_client;
	uint8_t m_clientid;
	
	private:
	int m_sockfd;

};
