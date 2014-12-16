## Start of the Makefile
all: client server 

client: physical.o datalink.o network.o client.o 
	g++ physical.o datalink.o network.o client.o -o client

server: physical.o datalink.o network.o server.o
	g++ physical.o datalink.o network.o server.o -o server
	
server.o: server.cpp server.h
	g++ -c server.cpp
	
client.o: client.cpp client.h
	g++ -c client.cpp

network.o: network.cpp network.h
	g++ -c network.cpp
	
datalink.o: datalink.cpp datalink.h
	g++ -c datalink.cpp
	
physical.o: physical.cpp physical.h
	g++ -c physical.cpp

clean:
	rm *.o
## End of the Makefile

