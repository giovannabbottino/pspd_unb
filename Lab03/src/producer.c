/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/

#include "utils.h" /* informacoes para rodar */

Mensagem * vetor;
Mensagem * receive_message;

Mensagem *createVetor(int tamanho);
amqp_connection_state_t conecta(char const * host, int port, char const * username, char const * password);
void fecha(amqp_connection_state_t conn);
void send_batch(amqp_connection_state_t conn, char const *routingkey, char const *exchange, Mensagem *message);

int main(int argc, char *argv[]) {
	char const *hostname;
	int port;
	char const *exchange;
	char const *routingkey;
	char const *username;
  	char const *password;
	amqp_connection_state_t conn;

	/* Verifica se a quantidade de servers foi enviado pelo argc  */
    if (argc<4) {
	  perror("[CLIENT] Modo de uso: producer host port exchange routingkey username password");
	  exit(0);       
    }
    /* Verifica se a quantidade de servers é um número */
 

	hostname = argv[1];
	port = atoi(argv[2]);
	exchange = argv[3]; 
	routingkey = argv[4]; 
	username = "guest";
	password = "guest";

	/* Cria o vetor da atividade */
	vetor = createVetor(VETOR);

	/* Cria e coneta no socket TCP */
	conn = conecta(hostname,port,username,password);
	/* Envia mensagem */
	send_batch(conn, routingkey, exchange ,vetor);
	/* Fecha conexao */
	fecha(conn);

	return 0;
}

/* Cria o vetor*/
Mensagem *createVetor(int tamanho){
	Mensagem * vetor = (Mensagem *) malloc(sizeof(Mensagem));
	vetor->tamanho = tamanho;
	vetor->vetor =  (float*)malloc ( vetor->tamanho * sizeof (float));

	for (int i=0; i<vetor->tamanho; i++){
		vetor->vetor[i] = pow((i - vetor->tamanho / 2), 2);
		vetor->vetor[i] = sqrt(vetor->vetor[i]);
	}

	return vetor;
}

amqp_connection_state_t conecta(char const * host, int port, char const * username, char const * password){
	amqp_connection_state_t conn = amqp_new_connection();

	amqp_socket_t *socket = NULL;
	socket = amqp_tcp_socket_new(conn);
	if (!socket) {
		die("Não pode criar o socket TCP");
	}
	
	int status = amqp_socket_open(socket, host, port);
	if (status) {
		die("Não pode conectar no Socket");
	}

	die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, username, password), "Logging in");
	amqp_channel_open(conn, 1);
	die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");
	
	return conn;
}

void fecha(amqp_connection_state_t conn){
	
	die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
	die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
	die_on_error(amqp_destroy_connection(conn), "Ending connection");
}

void send_batch(amqp_connection_state_t conn, char const *routingkey, char const *exchange, Mensagem *message){
	amqp_bytes_t message_bytes;
    message_bytes.len = sizeof(message->vetor);
    message_bytes.bytes = message->vetor;

    die_on_error(amqp_basic_publish(conn, 1, amqp_cstring_bytes(exchange),
                                    amqp_cstring_bytes(routingkey), 0, 0, NULL,
                                    message_bytes),
                 "Publishing");
}

void printMensagem(Mensagem *mensagem){
     if (mensagem->tamanho <= 0){
        printf("Mensagem vazia\n");
        return;
    }
    for(int i=0; i<mensagem->tamanho; i++){
        printf("%f\t", mensagem->vetor[i]);
    }
	printf("\n");
}