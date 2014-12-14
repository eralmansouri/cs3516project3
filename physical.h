/*
	Physical Layer emulation using TCP
	
	It has only a few functions.
		Open Connection
		Close Connection
		Send (without confirmation)
		Receive (without confirmation)
		
	Error checking and packet confirmations are left to other layers
*/

#include <string>
class Physical {
	public:
	Physical(std::string, uint16_t);
	int Send(char*, size_t);
	int Receive(char*, size_t);
	void Close();

	private:
	int m_sockfd;
	fd_set m_readfds;

};
