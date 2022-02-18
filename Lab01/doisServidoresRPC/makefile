all:
	mkdir -p bin/
	gcc src/server_1.c -o bin/server_1 -I./inc
	gcc src/server_2.c -o bin/server_2 -I./inc
	gcc src/client.c -o bin/client -I./inc -lm

run:
	bin/server_1 & bin/server_2 & bin/client 
clean:
	rm -rf bin/
vai: 
	make clean
	make all
	make run
