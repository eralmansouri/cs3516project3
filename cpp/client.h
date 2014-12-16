#include "network.h"

class Client : public Network {
	public:
	Client(char*, uint8_t, uint8_t);
	private:
	uint8_t m_id;
	void SendPhoto(char* filename);
};
