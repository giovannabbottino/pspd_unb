/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/
#include <properties.h>


Mensagem *createMensagem(Mensagem * receive_message);

int main(int argc, char *argv[]){
	int server, client;
	socklen_t client_size;
	float menor = 0, maior = 0;
	Mensagem * receive_message, * send_messages;

	struct sockaddr_in server_address, client_address; /* socket do servidor e cliente  */

	/* Verifica se a porta foi enviado pelo argc  */
    if (argc<2) {
	  printf("[SERVER] Digite a porta");
	  exit(0);       
    }

    /* Verifica se a porta é um número */
    if (!isdigit(*argv[1])){
        printf("[SERVER] A porta deve ser um número");
		exit(0);
    }

	int porta = atoi(argv[1]); 
	/* Criacao do socket TCP */
	server = socket(AF_INET, SOCK_STREAM, 0);
	if ( server == 0){ 
		/*
		* ARPA INTERNET PROTOCOLS -- IPv4
		* SOCK_STREAM orientado a conexão com transmissão de fluxos de bytes, sequencial e bidirecional
		* 0 - protocolo padrao para o tipo escolhido -- TCP
		*/
		printf("[SERVER %d] Criação do socket falhou", porta);
		exit(0);
	}
	printf("[SERVER %d] Criacao do socket TCP\n", porta);

	/* Preenchendo informacoes sobre o servidor */
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(HOST); 
	server_address.sin_port = htons(porta);

	/* Conecta na porta */
	if (bind(server, (struct sockaddr *)&server_address, sizeof(server_address))<0){
		printf("[SERVER %d] Não pode fazer bind na porta %d\n", porta, porta);
		exit(0);
	}
	printf("[SERVER %d] Conecta na porta\n", porta);

	/*  numero maximo de conexões */
	listen(server, 5);
	printf("[SERVER %d] Escutando\n", porta);

	/* Aceita conexoes direta entre o servidor e cliente */
	client_size = sizeof(client_address);
	client = accept(server, (struct sockaddr *) &client_address, &client_size);
	if ( client == -1 ){
		printf("[SERVER %d] Não pode dar accept na PORT", porta);
		exit(0);
	} 
	printf("[SERVER %d] Aceita conexoes direta\n", porta);

	printf("[SERVER %d] Esperando por dados no IP: %s, porta TCP numero: %d\n", porta, HOST, porta);
	int retorno_msg;
	/* Receber a mensagem */
	while(1){		
		retorno_msg = recv(client, &receive_message, sizeof(Mensagem), 0);
		
		if(retorno_msg == 0 || errno == 104){
			printf("[SERVER %d] Conexão Encerrada!\n", porta);
			close(client);
			exit(0);
		}else if (retorno_msg < 0){
			printf("[SERVER %d] Não foi possivel receber a mensagem", porta);
		} else{
			printf("[SERVER %d] Recebida a mensagem\n", porta);

			/* para responder */
			/* Descobre o menor e maior valor*/
			send_messages = createMensagem(receive_message);

			printf("[SERVER %d] Mensagem enviada [%f,%f]\n", porta, send_messages->vetor[0], send_messages->vetor[1]);
			if (send(client, &send_messages, sizeof(Mensagem), 0) < 0){
				printf("[SERVER %d] Não foi possivel responder a mensagem", porta);
			}
		}
	}
}

Mensagem *createMensagem(Mensagem * receive_message){
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

	return send_messages;
}
