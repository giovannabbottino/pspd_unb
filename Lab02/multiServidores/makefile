args = $(foreach a,$($(subst -,_,$1)_args),$(if $(value $a),$a="$($a)"))
serve_args = port

all:
	mkdir -p bin/
	gcc src/server.c -o bin/server -I./inc
	gcc src/client.c -o bin/client -I./inc -lm

clean:
	rm -rf bin/

run2:
	bin/server $(port1) & bin/server $(port2) & bin/client $(qtServer) $(port1) $(port2)
server2:
	make run2 port1=5065 port2=5075 qtServer=2

run:
	bin/server $(port1) & bin/client $(qtServer) $(port1)
server:
	make run port1=5065  qtServer=1