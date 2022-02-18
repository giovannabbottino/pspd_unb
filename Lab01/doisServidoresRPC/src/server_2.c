/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/
#include <properties.h>

int main(int argc, char *argv[]){
	int server, client;
	socklen_t client_size;
	float menor = 0, maior = 0, client_number;
	char receive_message[PDU], send_message[PDU];
	pid_t process_id; /*Process Identification*/

	struct sockaddr_in server_address, client_address; /* socket do servidor e cliente  */

	/* Criacao do socket TCP */
	server = socket(AF_INET, SOCK_STREAM, 0);
	if ( server == 0){ 
		/*
		* ARPA INTERNET PROTOCOLS -- IPv4
		* SOCK_STREAM orientado a conexão com transmissão de fluxos de bytes, sequencial e bidirecional
		* 0 - protocolo padrao para o tipo escolhido -- TCP
		*/
		perror("[SERVER 2] Criação do socket falhou");
		exit(0);
	}
	printf("[SERVER 2] Criacao do socket TCP\n");

	/* Preenchendo informacoes sobre o servidor */
	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = inet_addr(HOST); 
	server_address.sin_port = htons(PORT2);

	/* Conecta na porta */
	if (bind(server, (struct sockaddr *)&server_address, sizeof(server_address))<0){
		perror("[SERVER 2] Não pode fazer bind na porta PORT");
		exit(0);
	}
	printf("[SERVER 2] Conecta na porta\n");

	/*  numero maximo de conexões */
	listen(server, 5);
	printf("[SERVER 2] Escutando\n");

	/* Aceita conexoes direta entre o servidor e cliente */
	client_size = sizeof(client_address);
	client = accept(server, (struct sockaddr *) &client_address, &client_size);
	if ( client == -1 ){
		perror("[SERVER 2] Não pode dar accept na PORT");
		exit(0);
	} 
	printf("[SERVER 2] Aceita conexoes direta\n");

	printf("[SERVER 2] Esperando por dados no IP: %s, porta TCP numero: %d\n", HOST, PORT2);
	int retorno_msg;
	/* Receber a mensagem */
	while(1){
		bzero(receive_message, PDU); /* apaga a informacao*/
		
		retorno_msg = recv(client, receive_message, PDU, 0);
		
		if(retorno_msg == 0 || errno == 104){
			printf("[SERVER 2] Conexão Encerrada!\n");
			close(client);
			exit(0);
		}else if (retorno_msg < 0){
			perror("[SERVER 2] Não foi possivel receber a mensagem");
		} else{
			printf("[SERVER 2] Recebida a mensagem\n");

			/* para responder */
			bzero(send_message, PDU); /* apaga a informacao*/

			client_number = atof(receive_message);
			if (maior <= client_number){
				maior = client_number;
				gcvt(maior, 10, send_message);
			}
			if (menor >= client_number){
				menor = client_number;
				gcvt(menor, 10, send_message);
			}

			if (send(client, send_message, sizeof(send_message) + 1, 0) < 0){
				perror("[SERVER 2] Não foi possivel responder a mensagem");
			}
		}
	}
}
