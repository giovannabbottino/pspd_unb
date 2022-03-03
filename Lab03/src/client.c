/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/

#include <properties.h> /* informacoes para rodar */

Mensagem * vetor;
Mensagem * receive_message;

Mensagem *createVetor(int tamanho);
int conecta_client(int porta);
Mensagem *createMensagem(int tamanho, int inicio);
void comunicacao_client_server(int client, int porta, Mensagem * mensagem);
void printMensagem(Mensagem *mensagem);

int main(int argc, char *argv[]) {
	int qtServer;

	/* Verifica se a quantidade de servers foi enviado pelo argc  */
    if (argc<2) {
	  perror("[CLIENT] Digite a quantidade de servers");
	  exit(0);       
    }
    /* Verifica se a quantidade de servers é um número */
    if (!isdigit(*argv[1])){
        perror("[CLIENT] A quantidade de servers deve ser um número");
		exit(0);
    }

	qtServer = atoi(argv[1]); 
	printf("[CLIENT] Quantidade de server escolhidos: %d\n", qtServer);

	/* Cria o vetor da atividade */
	vetor = createVetor(VETOR);

	int portas[qtServer];
	for(int i=0; i<qtServer; i++){		
		/* Verifica se a porta é um número */
		if (!isdigit(*argv[i+2])){
			perror("[CLIENT] A porta deve ser um número");
			exit(0);
		}
		portas[i] = atoi(argv[i+2]);
	}

		
	Mensagem * mensagem;
	int client;
	for(int i=0; i<qtServer; i++){
		/*Fork para criar um novo processo*/
		if(fork() == 0){
			client = conecta_client(portas[i]);
			/* Divide o vetor em varias mensagens */
			int tamanho = vetor->tamanho/qtServer;
			for (int i = 0; i<vetor->tamanho; i+= tamanho){
				mensagem = createMensagem(tamanho, i);
			}
			comunicacao_client_server(client, portas[i], mensagem);
		} 
	}

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


int conecta_client(int porta){
	/* Criacao do socket TCP */
	int client = socket(AF_INET, SOCK_STREAM, 0);
	if (client == 0){ 
		/*
		* ARPA INTERNET PROTOCOLS -- IPv4
		* SOCK_STREAM orientado a conexão com transmissão de fluxos de bytes, sequencial e bidirecional
		* 0 - protocolo padrao para o tipo escolhido -- TCP
		*/
		perror("[CLIENT] Criação do socket falhou");
		exit(0);
	}
	printf("[CLIENT %d] Criacao do socket TCP\n", porta);

	struct sockaddr_in server; /* socket do servidor */

	/* Preenchendo informacoes sobre o cliente */
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = inet_addr(HOST); 
	server.sin_port = htons(porta);

	/* Inicia a conexão no socket */
	if (connect(client, (struct sockaddr *)&server, sizeof(server))<0){
		perror("[CLIENT] Não pode conectar no Socket");
		exit(0);
	}
	/* Para printar o tempo */
	printf("[CLIENT %d ] Inicia a conexão no socket\n", porta);

	printf("[CLIENT %d] Conectado no IP: %s, porta TCP numero: %d\n", porta, HOST, porta);

	return client;
}

Mensagem *createMensagem(int tamanho, int inicio){
	Mensagem * mensagem = (Mensagem *) malloc(sizeof(Mensagem));
	mensagem->tamanho = tamanho;
	mensagem->vetor =  (float*)malloc ( mensagem->tamanho * sizeof (float));

	for (int i=inicio; i<mensagem->tamanho; i++){
		mensagem->vetor[i] = vetor->vetor[i];
	}

	return vetor;
}

void comunicacao_client_server(int client, int porta, Mensagem * mensagem){
	/* Verifica se mensagem foi enviada */
	while(1){
		if(send(client, &mensagem, sizeof(Mensagem), 0) < 0){
			perror("[CLIENT] Falha no envio");
		} else{
			printf("[CLIENT %d] Mensagem enviada\n", porta);
		}

		if(recv(client, &receive_message, 2 * sizeof(Mensagem), 0) < 0){
			perror("[CLIENT] Falha ao receber resposta");
		}else{
			printf("[CLIENT %d] Mensagem recebida [%f,%f]\n", porta, receive_message[0], receive_message[1]);
			printf("[CLIENT %d] Encerrando conexão\n", porta);
			close(client);
			exit(0);
		}
	}	
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