all:
	mkdir -p bin/
	gcc src/server.c -o bin/server -I./inc
	gcc src/client.c -o bin/client -I./inc -lm

clean:
	rm -rf bin/

server :=  $(foreach wrd,$(port),bin/server $(wrd) &) 

run:
	$(server) bin/client $(qtServer) $(port)

worker:
	make run qtServer=1 port="5005"

worker2:
	make run qtServer=2 port="5005 5015"

worker4:
	make run qtServer=4 port="5005 5015 5025 5035"

worker6:
	make run qtServer=6 port="5005 5015 5025 5035 5045 5055"

worker8:
	make run qtServer=8 port="5005 5015 5025 5035 5045 5055 5065 5075"

worker10:
	make run qtServer=10 port="5005 5015 5025 5035 5045 5055 5065 5075 5085 5095"