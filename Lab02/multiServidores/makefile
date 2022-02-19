all :
	mkdir -p bin/
	gcc src/server.c -o bin/server -I./inc
	gcc src/client.c -o bin/client -I./inc -lm

clean :
	rm -rf bin/

portas = 5005 5015 5025 5035 5045 5055 5065 5075 5085 5095

port := $(wordlist 1, $(qtServer),$(portas))

servidor :=  $(foreach wrd,$(port),bin/server $(wrd) &) 

client: 
	bin/client $(qtServer) $(port)

server:
	$(servidor) 