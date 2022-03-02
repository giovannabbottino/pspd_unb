/*
* Giovanna Borges Bottino 170011267
* Felipe Boccardi Silva Agustini 180119818
*/

#include <properties.h> /* informacoes para rodar */

Mensagem * vetor;

Mensagem *createVetor(int tamanho);
Mensagem *createMensagem(int tamanho, int inicio);
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


	/* Divide o vetor em varias mensagens */
	int tamanho = vetor->tamanho/qtServer;
	Mensagem * mensagem;
	for (int i = 0; i<vetor->tamanho; i+= tamanho){
		mensagem = createMensagem(tamanho, i);
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

Mensagem *createMensagem(int tamanho, int inicio){
	Mensagem * mensagem = (Mensagem *) malloc(sizeof(Mensagem));
	mensagem->tamanho = tamanho;
	mensagem->vetor =  (float*)malloc ( mensagem->tamanho * sizeof (float));

	for (int i=inicio; i<mensagem->tamanho; i++){
		mensagem->vetor[i] = vetor->vetor[i];
	}

	return vetor;
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