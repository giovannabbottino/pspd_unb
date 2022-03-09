/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/

#include "utils.h"  /* informacoes para rodar */
float maior = 0,menor = 0;
amqp_connection_state_t * conecta(char const * hostname, int port, char const * username, char const * password){
	amqp_connection_state_t conn = amqp_new_connection();

	amqp_socket_t *socket = NULL;

	socket = amqp_tcp_socket_new(conn);
	if (!socket) {
		die("Não pode criar o socket TCP");
	}

	int status = amqp_socket_open(socket, hostname, port);
	if (status) {
		die("Não pode conectar no Socket");
	}

	die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, username, password), "Logging in");
	amqp_channel_open(conn, 1);
	die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");
	
	return conn;
}

void receive_batch(amqp_connection_state_t conn,char const *exchange,char const *bindingkey){
  amqp_bytes_t queuename;
  float float_converter;
  {
    amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 1, amqp_empty_bytes, 0, 0, 0, 1, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
    queuename = amqp_bytes_malloc_dup(r->queue);
    if (queuename.bytes == NULL) 
    {
      fprintf(stderr, "Sem memória ao copiar o nome da fila");
      return 0;
    }
  }

  amqp_queue_bind(conn, 1, queuename,amqp_cstring_bytes(exchange), amqp_cstring_bytes(bindingkey), amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Binding queue");

  amqp_basic_consume(conn, 1, queuename, amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");
  {
      amqp_rpc_reply_t res;
      amqp_envelope_t envelope;
	 
      amqp_maybe_release_buffers(conn);

      res = amqp_consume_message(conn, &envelope, NULL, 0);
	  amqp_message_t message;
      res = amqp_read_message(conn, NULL, &message, 0);
	  memcpy(&float_converter, &message.body, sizeof(float_converter));
	  
	  if(float_converter>maior){
		  maior = float_converter;
	  }else if (float_converter < menor){
		  menor = float_converter;
	  }
	  
	  if (AMQP_RESPONSE_NORMAL != res.reply_type) 
      {
        return;
      }
	  printf("a = %s", envelope.message.body.bytes);
      Mensagem *result = (Mensagem *)envelope.message.body.bytes;
      amqp_destroy_envelope(&envelope);
      return result;
  }
  amqp_bytes_free(queuename);
}

void printMensagem(){ 
	printf("maior:%f\tmenor:%f\n", maior,menor);
}

void fecha(amqp_connection_state_t conn){	
	die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
	die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
	die_on_error(amqp_destroy_connection(conn), "Ending connection");
}

int main(int argc, char *argv[]){
	char const *hostname;
	int port;
	char const *exchange;
  	char const *bindingkey;
	char const *username;
  	char const *password;
	amqp_connection_state_t conn;

	Mensagem * send_messages;

	struct sockaddr_in server_address, client_address; /* socket do servidor e cliente  */

	/* Verifica se a porta foi enviado pelo argc  */
    if (argc<5) {
	  printf("[SERVER] Modo de uso: consumer hostname port exchange bindingkey username password\n");
	  exit(0);       
    }

    /* Verifica se a porta é um número */
    if (!isdigit(*argv[2])){
        printf("[SERVER] A porta deve ser um número");
		exit(0);
    }

	hostname = argv[1];
	port = atoi(argv[2]);
	exchange = argv[3];  
	bindingkey =  argv[4];
	username = "guest";
	password = "guest";

	/* Criacao e conexao do socket TCP */
	conn = conecta(hostname, port, username, password);

	/* Recebe mensagem */
	receive_batch(conn,exchange,bindingkey); 
	printMensagem();


	/* Fecha conexao */
	fecha(conn);

	return 0;
}
