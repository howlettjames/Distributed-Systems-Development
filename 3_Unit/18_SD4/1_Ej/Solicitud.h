#ifndef SOLICITUD_H_
#define SOLICITUD_H_

#include <iostream>
#include <sys/time.h>
#include "SocketDatagrama.h"
#include "mensaje.h"

class Solicitud
{
	public:
		Solicitud();
		~Solicitud();
		char * doOperation(char *IP, int puerto, int operationId, char *arguments, int args_size);

	private:
		SocketDatagrama *socketlocal;
		static unsigned int requestId;
};

#endif