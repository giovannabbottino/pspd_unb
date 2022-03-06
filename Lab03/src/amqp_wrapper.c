#include <properties.h> /* informacoes para rodar */

amqp_connection_state_t * conecta(char const * host, int port, char const * username, char const * password){
	conn = amqp_new_connection();

	socket = amqp_tcp_socket_new(conn);
	if (!socket) {
		die("Não pode criar o socket TCP");
	}

	status = amqp_socket_open(socket, host, port);
	if (status) {
		die("Não pode conectar no Socket");
	}

	die_on_amqp_error(amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, username, password), "Logging in");
	amqp_channel_open(conn, 1);
	die_on_amqp_error(amqp_get_rpc_reply(conn), "Opening channel");
	
	return conn;
}

void fecha(amqp_connection_state_t conn){
	die_on_amqp_error(amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS), "Closing channel");
	die_on_amqp_error(amqp_connection_close(conn, AMQP_REPLY_SUCCESS), "Closing connection");
	die_on_error(amqp_destroy_connection(conn), "Ending connection");
}

void send_batch(amqp_connection_state_t conn,char const *exchange,char const *routingkey,char const *messagebody){
    amqp_basic_properties_t props;
    props._flags = AMQP_BASIC_CONTENT_TYPE_FLAG | AMQP_BASIC_DELIVERY_MODE_FLAG;
    props.content_type = amqp_bytes_t("text/plain");
    props.delivery_mode = 2; /* persistent delivery mode */
    die_on_error(amqp_basic_publish(conn, 1, amqp_bytes_t(exchange), amqp_bytes_t(routingkey), 0, 0, &props, amqp_bytes_t(messagebody)), "Publishing");
}

Mensagem * receive_batch(amqp_connection_state_t conn,char const *exchange,char const *bindingkey){
  amqp_bytes_t queuename;
  {
    amqp_queue_declare_ok_t *r = amqp_queue_declare(conn, 1, amqp_empty_bytes, 0, 0, 0, 1, amqp_empty_table);
    die_on_amqp_error(amqp_get_rpc_reply(conn), "Declaring queue");
    queuename = amqp_bytes_malloc_dup(r->queue);
    if (queuename.bytes == NULL) 
    {
      fprintf(stderr, "Sem memória ao copiar o nome da fila");
      return 0;
    }
  }

  amqp_queue_bind(conn, 1, queuename, amqp_bytes_t(exchange), amqp_bytes_t(bindingkey), amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Binding queue");

  amqp_basic_consume(conn, 1, queuename, amqp_empty_bytes, 0, 1, 0, amqp_empty_table);
  die_on_amqp_error(amqp_get_rpc_reply(conn), "Consuming");

  {
    //for (;;) 
    //{
      amqp_rpc_reply_t res;
      amqp_envelope_t envelope;

      amqp_maybe_release_buffers(conn);

      res = amqp_consume_message(conn, &envelope, NULL, 0);

      if (AMQP_RESPONSE_NORMAL != res.reply_type) 
      {
        return;
      }
      Mensagem *result = (Mensagem *)malloc( envelope.message.body.len);
      amqp_destroy_envelope(&envelope);
      return result;
    //}
  }
}

void printMensagem(Mensagem *mensagem){
     if (mensagem->tamanho <= 0){
        printf("Mensagem vazia\n");
        return;
    }
    for(int i=0; i<mensagem->tamanho; i++){
        printf("%f\t", mensagem->vetor[i]);
    }
	printf("]\n");
}