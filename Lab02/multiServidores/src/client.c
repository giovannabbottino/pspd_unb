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
	printf("[CLIENT] Criacao do socket TCP\n");

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
	printf("[CLIENT] Inicia a conexão no socket\n");

	printf("[CLIENT] Conectado no IP: %s, porta TCP numero: %d\n", HOST, porta);

	return client;
}

void comunicacao_client_server(int client){
	/* Verifica se mensagem foi enviada */
	while(1){
		if(send(client, &vetor, VETOR * sizeof(float), 0) < 0){
			perror("[CLIENT] Falha no envio");
		} else{
			printf("[CLIENT] Mensagem enviada\n");
		}

		if(recv(client, &receive_message, 2 * sizeof(float), 0) < 0){
			perror("[CLIENT] Falha ao receber resposta");
		}

		if (menor == receive_message[0] && maior == receive_message[1]){
			printf("[CLIENT] Menor valor recebido está correto: %f\n", receive_message[0]);
			printf("[CLIENT] Maior valor recebido está correto: %f\n", receive_message[1]);
			close(client);
			exit(0);
		}
	}	
}
	
int main(int argc, char *argv[]) {
	int client;
	int qtServer;
	pid_t process_id; /*Process Identification*/


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
		printf("[CLIENT] Verifica se a porta é um número\n");
		portas[i] = atoi(argv[i + 2]);
	}

	
	for (int i=0; i<qtServer/2; i++){
		/*Fork para criar um novo processo*/
		process_id=fork();
		if(process_id==0){
			client = conecta_client(portas[i]);
			comunicacao_client_server(client);
		} else{
			client = conecta_client(portas[i+2]);
			comunicacao_client_server(client);
		}
	}

	return 0;
}
