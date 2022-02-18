/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/

#include <properties.h> /* informacoes para rodar */

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

int main(int argc, char *argv[]) {
	int client_1, client_2;
	char server_message[PDU], receive_message[PDU], send_message[PDU];
	pid_t process_id; /*Process Identification*/
	float vetor[VETOR], menor = 0, maior = 0, server_number;
	int count_correct_number_client_1 = 0, count_correct_number_client_2 = 0;
	
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

	client_1 = conecta_client(PORT1);
	client_2 = conecta_client(PORT2);

	/* Verifica se mensagem foi enviada */
	for (int i=0; i<VETOR; i++){
		bzero(send_message, PDU); /* apaga a informacao*/

		if (count_correct_number_client_1 >= 2 || i == VETOR - 1){
			close(client_1);
			break;
		} else {
			gcvt(vetor[i], 10, send_message);
		}

		if(send(client_1, send_message, sizeof(send_message), 0) < 0){
			perror("[CLIENT] Falha no envio");
		} else{
			printf("[CLIENT] Mensagem enviada\n");
		}

		bzero(server_message, PDU); /* apaga a informacao*/
		if(recv(client_1, server_message, sizeof(server_message), 0) < 0){
			perror("[CLIENT] Falha ao receber resposta");
		}

		server_number = atof(server_message);
		if (maior == server_number){
			printf("[CLIENT] Maior valor recebido de server 1 está correto: %f\n",server_number);
			count_correct_number_client_1++;
		}
		if (menor == server_number){
			printf("[CLIENT] Menor valor recebido de server 1 está correto: %f\n",server_number);
			count_correct_number_client_1++;
		}

		bzero(send_message, PDU); /* apaga a informacao*/

		if (count_correct_number_client_2 >= 2 || i == VETOR - 1){
			close(client_2);
			break;
		} else {
			gcvt(vetor[i], 10, send_message);
		}

		if(send(client_2, send_message, sizeof(send_message), 0) < 0){
			perror("[CLIENT] Falha no envio");
		} else{
			printf("[CLIENT] Mensagem enviada\n");
		}

		bzero(server_message, PDU); /* apaga a informacao*/
		if(recv(client_2, server_message, sizeof(server_message), 0) < 0){
			perror("[CLIENT] Falha ao receber resposta");
		}

		server_number = atof(server_message);
		if (maior == server_number){
			printf("[CLIENT] Maior valor recebido de server 2 está correto: %f\n",server_number);
			count_correct_number_client_2++;
		}
		if (menor == server_number){
			printf("[CLIENT] Menor valor recebido de server 2 está correto: %f\n",server_number);
			count_correct_number_client_2++;
		}
	}
	return 0;
}
