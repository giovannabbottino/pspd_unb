struct vetor{
    int tamanho;
    float * vetor; 
};
program PROG { 
    version VERSAO { 
        float PROCURA(vetor) = 1; 
    } = 01;
} = 5000;