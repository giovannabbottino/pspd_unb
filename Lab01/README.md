# Lab 01 Construindo aplicações distribuídas usando RPC

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)  ![C](https://img.shields.io/badge/Solutions-blue.svg?style=flat&logo=c)

## Versão com um 1 cliente e um worker (servidor RPC) 

No ambiente local (Linux), construir uma pequena aplicação distribuída (linguagem C), que descobre o maior e o menor valor em um vetor de 10 posições.

## Versão com dois workers (função instanciada em dois servidores RPC) 

Nesse versão, a aplicação deve ser construída, de modo que o cliente faça a solicitação envolvendo pelo menos dois servidores (workers)

## Rodando

Para rodar basta utilizar o comando 
```
make all
```

e em seguida o 
```
make run
```

Se precisar use 
```
make clean
``` 

Para limpar a pasta.

 
