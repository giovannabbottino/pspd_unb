all:
	mkdir -p bin/
	gcc src/consumer.c -o bin/consumer -I./inc -lm
	gcc src/producer.c -o bin/producer -I./inc -lm

clean:
	rm -rf bin/

run: 
	bin/producer 127.0.0.1 5055 exchange routingkey username password
	bin/consumer 127.0.0.1 5055 exchange bindingkey username password