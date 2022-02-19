/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/
#include <properties.h>

int cmpfunc (const void * a, const void * b) {
   return ( *(float*)a - *(float*)b );
}

int main(int argc, char *argv[]){
	int server, client;
	socklen_t client_size;
	float menor = 0, maior = 0;
	float vetor[VETOR] = {0}, send_message[2] = {0};
	time_t rawtime;
	struct tm * timeinfo;

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
		retorno_msg = recv(client, &vetor, VETOR * sizeof(float), 0);
		/* Para printar o tempo */
		time ( &rawtime );
		timeinfo = localtime ( &rawtime );
		
		if(retorno_msg == 0 || errno == 104){
			printf("[SERVER %d | %d:%d:%d ] Conexão Encerrada!\n", porta, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			close(client);
			exit(0);
		}else if (retorno_msg < 0){
			printf("[SERVER %d | %d:%d:%d ] Não foi possivel receber a mensagem", porta, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
		} else{
			printf("[SERVER %d | %d:%d:%d ] Recebida a mensagem\n", porta, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);

			/* para responder */
			/* Descobre o menor e maior valor*/
			for (int i=0; i<VETOR-1; i++){
				if (vetor[i] >= maior){
					maior = vetor[i];
				}
				if (vetor[i] <= menor){
					menor = vetor[i];
				}
			}

			send_message[0] = menor;
			send_message[1] = maior;
			printf("[SERVER %d | %d:%d:%d ] Mensagem enviada [%f,%f]\n", porta,timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, send_message[0], send_message[1]);
			if (send(client, &send_message, VETOR * sizeof(float) + 1, 0) < 0){
				printf("[SERVER %d | %d:%d:%d ] Não foi possivel responder a mensagem", porta, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
			}
		}
	}
}
