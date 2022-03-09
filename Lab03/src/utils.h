// Copyright 2007 - 2021, Alan Antonuk and the rabbitmq-c contributors.
// SPDX-License-Identifier: mit

#ifndef librabbitmq_utils_h
#include <stdio.h> /* printf() */
#include <stdlib.h>
#include <ctype.h> /* isdigit*/
#include <math.h> /* sqrt() e pow() */

#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include <rabbitmq-c/framing.h>

#define VETOR 100

typedef struct mensagem{
    int tamanho;
    float * vetor;
}Mensagem;


void die(const char *fmt, ...);
extern void die_on_error(int x, char const *context);
extern void die_on_amqp_error(amqp_rpc_reply_t x, char const *context);

extern void amqp_dump(void const *buffer, size_t len);

extern uint64_t now_microseconds(void);
extern void microsleep(int usec);

#endif
