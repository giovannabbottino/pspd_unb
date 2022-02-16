# Lab02 – Construindo aplicações distribuídas usando sockets TCP/UDP

[![License: GPL v3](https://img.shields.io/badge/License-GPLv3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)  ![C](https://img.shields.io/badge/Solutions-blue.svg?style=flat&logo=c)

## Versão com um 1 cliente e 1 worker (servidor)

No ambiente local (Linux), construir uma pequena aplicação distribuída (linguagem C), que descobre o maior e o menor valor em um vetor de 10 posições.

### Rodando

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


## Versão envolvendo 1 cliente e múltiplos workers (servidores) 

Nessa versão, a aplicação deve ser construída de modo que o cliente faça solicitação simultânea aos workers, visando melhoria de performance da aplicação.

 
