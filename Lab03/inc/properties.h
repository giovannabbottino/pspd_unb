#ifndef PROPERTIES
#include <stdio.h> /* printf(), perror() */
#include <stdlib.h>
#include <time.h> /* time_t e struct tm */
#include <netinet/in.h> /* struct sockaddr_in */
#include <sys/socket.h> /* connect() send() bind() recv() */
#include <arpa/inet.h>  /* htons(), inet_addr() */
#include <sys/types.h> /* AF_INET, SOCK_STREAM pid_t */
#include <strings.h> /* bzero() */
#include <ctype.h> /* isdigit*/
#include <unistd.h>  /* fork()*/
#include <errno.h> /* errno */
#include <math.h> /* sqrt() e pow() */

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>

#define HOST "127.0.0.1"

#define VETOR 100

typedef struct mensagem{
    int tamanho;
    float * vetor;
}Mensagem;

#endif
