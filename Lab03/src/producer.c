/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/

#include "amqp_wrapper.c"

Mensagem * vetor;
Mensagem * receive_message;

Mensagem *createVetor(int tamanho);

int main(int argc, char *argv[]) {
	char const *hostname;
	int port;
	char const *exchange;
	char const *routingkey;
	char const *username;
  	char const *password;
	amqp_connection_state_t conn;

	/* Verifica se a quantidade de servers foi enviado pelo argc  */
    if (argc<6) {
	  perror("[CLIENT] Modo de uso: producer host port exchange routingkey username password");
	  exit(0);       
    }
    /* Verifica se a quantidade de servers é um número */
    if (!isdigit(*argv[2]) || !isdigit(*argv[3])){
        perror("[CLIENT] Os argumentos devem ser um número");
		exit(0);
    }

	hostname = argv[1];
	port = atoi(argv[2]);
	exchange = argv[3];
	routingkey = argv[4];
	username = argv[5];
	password = argv[6];

	/* Cria o vetor da atividade */
	vetor = createVetor(VETOR);

	/* Cria e coneta no socket TCP */
	conn = create_messagebus_context(hostname,port,username,password);
	/* Envia mensagem */
	send_batch(conn,exchange,routingkey,vetor);
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