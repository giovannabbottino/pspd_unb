/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/

#include "amqp_wrapper.c"

Mensagem *encontraValores(Mensagem * receive_message);

int main(int argc, char *argv[]){
	char const *hostname;
	int port;
	char const *exchange;
	char const *bindingkey;
	char const *username;
  	char const *password;
	amqp_connection_state_t conn;

	amqp_bytes_t queuename;

	Mensagem * send_messages;

	struct sockaddr_in server_address, client_address; /* socket do servidor e cliente  */

	/* Verifica se a porta foi enviado pelo argc  */
    if (argc<7) {
	  printf("[SERVER] Modo de uso: consumer host port exchange bindingkey username password\n");
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
	bindingkey = argv[4];
	username = argv[5];
	password = argv[6];

	/* Criacao e conexao do socket TCP */
	conn = conexao(host, port);

	/* Recebe mensagem */
	Mensagem * receive_message = receive_batch(conn,exchange,bindingkey); 

	/* Encontra valores do vetor */
	Mensagem * send_messages = encontraValores(receive_message);

	printMensagem(send_messages);

	/* Fecha conexao */
	fecha(conn);

	return 0;
}

Mensagem * encontraValores(Mensagem * receive_message){
	Mensagem * send_messages = (Mensagem *) malloc(sizeof(Mensagem));
	send_messages->tamanho = 2;
	send_messages->vetor =  (float*)malloc ( send_messages->tamanho * sizeof (float));
	int menor = 0, maior = 0;

	for (int i=0; i<receive_message->tamanho; i++){
		if (receive_message->vetor[i] >= maior){
			maior = receive_message->vetor[i];
		}
		if (receive_message->vetor[i] <= menor){
			menor = receive_message->vetor[i];
		}
	}
	send_messages->vetor[0] = menor;
	send_messages->vetor[1] = menor;

	return send_messages;
}
