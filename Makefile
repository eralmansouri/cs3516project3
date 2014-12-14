## Start of the Makefile
all: main

main: physical.o main.o datalink.o
	g++ main.o physical.o datalink.o -o main
	
main.o: main.cpp
	g++ -c main.cpp
	
datalink.o: datalink.cpp datalink.h
	g++ -c datalink.cpp
	
physical.o: physical.cpp physical.h
	g++ -c physical.cpp


clean:
	rm *.o
## End of the Makefile

