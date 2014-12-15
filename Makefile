default: all

all: server client

server: server.c physical.c datalink.c util.c
	gcc -o server server.c physical.c datalink.c util.c

client: client.c physical.c datalink.c util.c
	gcc -o client client.c physical.c datalink.c util.c

clean:
	rm server client
