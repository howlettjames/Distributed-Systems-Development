#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include <iostream>
#include "SocketDatagrama.h"
#include "mensaje.h"

// CREAMOS UN STRUCT CLIENTE PARA ALMACENAR LA INFO DE CADA UNO
struct cliente
{
	char ip[16];
	int puerto;
	unsigned int requestId;
};

class Respuesta
{
	public:
		Respuesta(int pl);
		~Respuesta();
		struct mensaje *getRequest(void);
		void sendReply(char *respuesta);

	private:
		SocketDatagrama *socketlocal;
		struct mensaje message_recv;
		char ip[16];
		int puerto;
		static unsigned int requestId;
};

#endif