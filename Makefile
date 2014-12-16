default: all

all: server client

server: server.c physical.c datalink.c util.c log.h
	gcc -o server server.c physical.c datalink.c util.c -lpthread

client: client.c physical.c datalink.c util.c log.h
	gcc -o client client.c physical.c datalink.c util.c

clean:
	rm server client
