/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/

#include <properties.h> /* informacoes para rodar */

int main(int argc, char *argv[]) {
	int client;
	struct sockaddr_in server_address; /* socket do servidor */
	pid_t process_id; /*Process Identification*/
	float server_message[PDU], receive_message[PDU], send_message[PDU];
	float vetor[VETOR], menor = 0, maior = 0;
	
	/* Cria e descobre o menor e maior valor*/
	for (int i=0; i<VETOR; i++){
		send_message[i] = pow((i - VETOR / 2), 2);
		send_message[i] = sqrt(send_message[i]);
		if (send_message[i] > maior){
			maior = send_message[i];
		}
		if (send_message[i] > menor){
			menor = send_message[i];
		}
	}


	/* Criacao do socket TCP */
	client = socket(AF_INET, SOCK_STREAM, 0);
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

	/* Preenchendo informacoes sobre o cliente */
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(HOST); 
	server_address.sin_port = htons(PORT);

	/* Inicia a conexão no socket */
	if (connect(client, (struct sockaddr *)&server_address, sizeof(server_address))<0){
		perror("[CLIENT] Não pode conectar no Socket");
		exit(0);
	}
	printf("[CLIENT] Inicia a conexão no socket\n");

	printf("[CLIENT] Conectado no IP: %s, porta TCP numero: %d\n", HOST, PORT);
	
	/* Verifica se mensagem foi enviada */
	int counter=0;
	while(1){
		bzero(server_message, PDU); /* apaga a informacao*/
		if(send(client, send_message, sizeof(send_message), 0) < 0){
			perror("[CLIENT] Falha no envio");
		} else{
			printf("[CLIENT] Mensagem enviada\n");
		}

		if(recv(client, server_message, sizeof(server_message), 0) < 0){
			perror("[CLIENT] Falha ao receber resposta");
		}

		if (menor == server_message[0] && server_message[1]){
			if (maior == server_message[0]){
				printf("[CLIENT] Menor valor recebido está correto: %f\n",server_message[0]);
			}
			if (maior == server_message[1]){
				printf("[CLIENT] Maior valor recebido está correto: %f\n",server_message[1]);
			}
			exit(0);	
		}

		if (counter>=3){
			perror("[CLIENT] Mensagem não pode ser enviada");
			printf("[CLIENT] Mensagem cancelada\n");
			break;
		}
	}
}
