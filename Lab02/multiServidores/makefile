args = $(foreach a,$($(subst -,_,$1)_args),$(if $(value $a),$a="$($a)"))
serve_args = port

all:
	mkdir -p bin/
	gcc src/server.c -o bin/server -I./inc
	gcc src/client.c -o bin/client -I./inc -lm

run:
	bin/server $(port1) & bin/server $(port2) & bin/client $(qtServer) $(port1) $(port2)
clean:
	rm -rf bin/
server2: 
	make clean
	make all 
	make run qtServer=2 port1=5050 port2=5055

