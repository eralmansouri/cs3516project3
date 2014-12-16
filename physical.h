//Author: Essam Al-Mansouri
#define LOG outlogfile << "Client (" << m_clientid*1 << "): " 
#include <fstream>
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
	std::ofstream outlogfile;

	bool is_client; //true for client application only
	uint8_t m_clientid;
	
	private:
	int m_sockfd;

};
