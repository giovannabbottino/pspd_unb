/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/
#include <properties.h>

int main(int argc, char *argv[]){
	int server, client;
	socklen_t client_size;
	float menor = 0, maior = 0, client_number;
	float vetor[VETOR] = {0}, send_message[2] = {0};

	struct sockaddr_in server_address, client_address; /* socket do servidor e cliente  */

	/* Criacao do socket TCP */
	server = socket(AF_INET, SOCK_STREAM, 0);
	if ( server == 0){ 
		/*
		* ARPA INTERNET PROTOCOLS -- IPv4
		* SOCK_STREAM orientado a conexão com transmissão de fluxos de bytes, sequencial e bidirecional
		* 0 - protocolo padrao para o tipo escolhido -- TCP
		*/
		perror("[SERVER] Criação do socket falhou");
		exit(0);
	}
	printf("[SERVER] Criacao do socket TCP\n");

	/* Preenchendo informacoes sobre o servidor */
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(HOST); 
	server_address.sin_port = htons(PORT);

	/* Conecta na porta */
	if (bind(server, (struct sockaddr *)&server_address, sizeof(server_address))<0){
		perror("[SERVER] Não pode fazer bind na porta PORT");
		exit(0);
	}
	printf("[SERVER] Conecta na porta\n");

	/*  numero maximo de conexões */
	listen(server, 5);
	printf("[SERVER] Escutando\n");

	/* Aceita conexoes direta entre o servidor e cliente */
	client_size = sizeof(client_address);
	client = accept(server, (struct sockaddr *) &client_address, &client_size);
	if ( client == -1 ){
		perror("[SERVER] Não pode dar accept na PORT");
		exit(0);
	} 
	printf("[SERVER] Aceita conexoes direta\n");

	printf("[SERVER] Esperando por dados no IP: %s, porta TCP numero: %d\n", HOST, PORT);
	int retorno_msg;
	/* Receber a mensagem */
	while(1){		
		retorno_msg = recv(client, &vetor, VETOR * sizeof(float), 0);
		
		if(retorno_msg == 0 || errno == 104){
			printf("[SERVER] Conexão Encerrada!\n");
			close(client);
			exit(0);
		}else if (retorno_msg < 0){
			perror("[SERVER] Não foi possivel receber a mensagem");
		} else{
			printf("[SERVER] Recebida a mensagem\n");

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
				perror("[SERVER] Não foi possivel responder a mensagem");
			}
		}
	}
}
