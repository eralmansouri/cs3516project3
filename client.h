#include "network.h"

class Client : public Network {
	public:
	Client(char*, uint8_t, uint8_t);
	private:
	void SendPhoto(char* filename);
};
