/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/

#include <properties.h> /* informacoes para rodar */

float vetor[VETOR], menor = 0, maior = 0, receive_message[2];

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
	printf("[CLIENT %d] Inicia a conexão no socket\n", porta);

	printf("[CLIENT %d] Conectado no IP: %s, porta TCP numero: %d\n", porta, HOST, porta);

	return client;
}

void comunicacao_client_server(int client, int porta){
	/* Verifica se mensagem foi enviada */
	while(1){
		if(send(client, &vetor, VETOR * sizeof(float), 0) < 0){
			perror("[CLIENT] Falha no envio");
		} else{
			printf("[CLIENT %d] Mensagem enviada\n", porta );
		}

		if(recv(client, &receive_message, 2 * sizeof(float), 0) < 0){
			perror("[CLIENT] Falha ao receber resposta");
		}

		printf("[CLIENT %d] Mensagem recebida [%f,%f]\n", porta, receive_message[0], receive_message[1]);
		if (menor == receive_message[0] && maior == receive_message[1]){
			printf("[CLIENT %d] Menor valor recebido está correto: %f\n", porta, receive_message[0]);
			printf("[CLIENT %d] Maior valor recebido está correto: %f\n", porta, receive_message[1]);
			close(client);
			exit(0);
		}
	}	
}
	
int main(int argc, char *argv[]) {
	int client;
	int qtServer;

	/* Cria e descobre o menor e maior valor*/
	for (int i=0; i<VETOR; i++){
		vetor[i] = pow((i - VETOR / 2), 2);
		vetor[i] = sqrt(vetor[i]);

		if (vetor[i] >= maior){
			maior = vetor[i];
		}
		if (vetor[i] <= menor){
			menor = vetor[i];
		}
	}

	/* Verifica se a quantidade de servers foi enviado pelo argc  */
    if (argc<2) {
	  perror("[CLIENT] Digite a quantidade de servers");
	  exit(0);       
    }
    printf("[CLIENT] Verifica se a quantidade de servers foi enviada pelo argc\n");
    
    /* Verifica se a quantidade de servers é um número */
    if (!isdigit(*argv[1])){
        perror("[CLIENT] A quantidade de servers deve ser um número");
		exit(0);
    }
    printf("[CLIENT] Verifica se a quantidade de servers é um número\n");

	qtServer = atoi(argv[1]); 
	printf("[CLIENT] Quantidade de server escolhidos: %d\n", qtServer);

	/* Verifica se a quantidade de portas foi enviado pelo argc  */
	if (argc<qtServer+2) {
		perror("[CLIENT] Digite todas as portas");
		exit(0);       
	}
	printf("[CLIENT] Verifica se a porta foi enviada pelo argc\n");
	
	int portas[qtServer];
	for(int i=0; i<qtServer; i++){		
		/* Verifica se a porta é um número */
		if (!isdigit(*argv[i+2])){
			perror("[CLIENT] A porta deve ser um número");
			exit(0);
		}
		printf("[CLIENT] Verifica se a porta %d é um número\n", atoi(argv[i+2]));
		portas[i] = atoi(argv[i+2]);
	}

	for(int i=0; i<qtServer; i++){
		/*Fork para criar um novo processo*/
		if(fork() == 0){
			client = conecta_client(portas[i]);
			comunicacao_client_server(client, portas[i]);
		} 
	}
}
