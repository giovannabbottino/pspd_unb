/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/

#include "utils.h"  /* informacoes para rodar */
#include "cJSON.h"
#include <string.h>

amqp_connection_state_t * conecta(char const * hostname, int port, char const * username, char const * password);
Mensagem * receive_batch(amqp_connection_state_t conn,char const *exchange,char const *bindingkey);
Mensagem * createVetorByJSON(char * message);
void encontraValores(Mensagem * receive_message);
void fecha(amqp_connection_state_t conn);

int main(int argc, char *argv[]){
	char const *hostname;
	int port;
	char const *exchange;
  	char const *bindingkey;
	char const *username;
  	char const *password;
	amqp_connection_state_t conn;

	/* Verifica se a porta foi enviado pelo argc  */
  if (argc<7) {
	  printf("Modo de uso: consumer hostname port exchange bindingkey username password\n");
	  exit(0);       
  }

  /* Verifica se a porta é um número */
  if (!isdigit(*argv[2])){
        printf("A porta deve ser um número");
		exit(0);
  }

	hostname = argv[1];
	port = atoi(argv[2]);
	exchange = argv[3];  
	bindingkey =  argv[4];
	username = argv[5];
	password = argv[6];

	/* Criacao e conexao do socket TCP */
	conn = conecta(hostname, port, username, password);

	/* Recebe mensagem */
	Mensagem * receive_message = receive_batch(conn, exchange, bindingkey);

	/* acha o menor e maior valor e printa */ 
	encontraValores(receive_message);

	/* Fecha conexao */
	fecha(conn);

	return 0;
}

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

Mensagem * receive_batch(amqp_connection_state_t conn,char const *exchange,char const *bindingkey){
  Mensagem * receive_message;
  amqp_frame_t frame;

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
    amqp_rpc_reply_t ret;
    amqp_envelope_t envelope;

    amqp_maybe_release_buffers(conn);
    ret = amqp_consume_message(conn, &envelope, NULL, 0);

    /* gera uma struct apartir do JSON*/
    receive_message = createVetorByJSON((char *)envelope.message.body.bytes);

    if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
      return;
    }
  }
  amqp_bytes_free(queuename);

  return receive_message;
}

Mensagem * createVetorByJSON(char * message){
	Mensagem * receive_message = (Mensagem *) malloc(sizeof(Mensagem));
	cJSON * root, * vetor, * tamanho, * item;
	root = cJSON_Parse(message);

	tamanho = cJSON_GetObjectItem(root, "tamanho");

	vetor = cJSON_GetObjectItem(root, "vetor");

	receive_message->tamanho = tamanho->valueint;

	receive_message->vetor = (float*)malloc(receive_message->tamanho * sizeof (float));

	for (int i = 0; i<receive_message->tamanho; i++){
		receive_message->vetor[i] = cJSON_GetArrayItem(vetor, i)->valuedouble;
	}

  return receive_message;
}

void encontraValores(Mensagem * receive_message){
	float menor = 0, maior = 0;

	for (int i=0; i<receive_message->tamanho; i++){
		if (receive_message->vetor[i] >= maior){
			maior = receive_message->vetor[i];
		}
		if (receive_message->vetor[i] <= menor){
			menor = receive_message->vetor[i];
		}
	}
  printf("Menor %f\tMaior %f\n", menor, maior);
}

void fecha(amqp_connection_state_t conn){	
	die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
	die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
	die_on_error(amqp_destroy_connection(conn), "Ending connection");
}
