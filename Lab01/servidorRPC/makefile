all: src/server.c src/client.c
	mkdir -p bin/
	gcc src/server.c -o bin/server -I./inc
	gcc src/client.c -o bin/client -I./inc -lm

run:
	bin/server & bin/client 
clean:
	rm -rf bin/

