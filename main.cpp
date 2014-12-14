#include "datalink.h"

int main(int argc , char *argv[]){

	std::string serverurl("google.com");
	DataLink testp(serverurl, 80);
	testp.Send(new char[1], 0);
	return 0;
}
