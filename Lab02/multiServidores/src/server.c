/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/
#include <properties.h>

int main(int argc, char *argv[]){
	int server, client;
	socklen_t client_size;
	float menor = 0, maior = 0, client_number;
	float vetor[VETOR], send_message[2];

	struct sockaddr_in server_address, client_address; /* socket do servidor e cliente  */

	/* Verifica se a porta foi enviado pelo argc  */
    if (argc<2) {
	  printf("[SERVER] Digite a porta");
	  exit(0);       
    }
    printf("[SERVER] Verifica se a porta foi enviada pelo argc\n");
    
    /* Verifica se a porta é um número */
    if (!isdigit(*argv[1])){
        printf("[SERVER] A porta deve ser um número");
		exit(0);
    }
    printf("[SERVER] Verifica se a porta é um número\n");

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
		printf("[SERVER %d] Não pode fazer bind na porta PORT", porta);
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
		retorno_msg = recv(client, &vetor, VETOR * sizeof(float), 0);
		
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
			for (int i=0; i<VETOR; i++){
				if (vetor[i] >= maior){
					maior = vetor[i];
				}
				if (vetor[i] <= menor){
					menor = vetor[i];
				}
			}

			send_message[0] = menor;
			send_message[1] = maior;

			if (send(client, &send_message, VETOR * sizeof(float) + 1, 0) < 0){
				printf("[SERVER %d] Não foi possivel responder a mensagem", porta);
			}
		}
	}
}
