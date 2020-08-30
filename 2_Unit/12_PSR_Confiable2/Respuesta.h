#ifndef SERVIDOR_H_
#define SERVIDOR_H_

#include <iostream>
#include "SocketDatagrama.h"
#include "mensaje.h"

// EL SERVIDOR MANTENDRÁ UNA BD DE HASTA 10 CLIENTES
#define CAPACIDAD_CLIENTES 10

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
		struct cliente clientes[CAPACIDAD_CLIENTES];
		int ncliente = 0;
};

#endif