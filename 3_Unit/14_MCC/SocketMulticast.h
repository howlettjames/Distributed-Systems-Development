#ifndef SOCKETMULTICAST_H_
#define SOCKETMULTICAST_H_

#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h> //memcpy
#include <arpa/inet.h> //inet_pton
#include "PaqueteDatagrama.h"
#include "mensaje.h"

using namespace std;

class SocketMulticast
{
	public:
		SocketMulticast(int);
		~SocketMulticast();
		int recibe(PaqueteDatagrama &p);
		int recibeTimeout(PaqueteDatagrama &p, time_t segundos, suseconds_t microsegundos);
		int recibeConfiable(PaqueteDatagrama &p);
		int envia(PaqueteDatagrama &p, unsigned char ttl);
		int enviaConfiable(PaqueteDatagrama &p, int ttl, unsigned char num_receptores);
		void unirseGrupo(char *);
		void salirseGrupo(char *);
		static int getIdentificadorMensaje();

	private:
		int s;
		struct sockaddr_in direccion_local, direccion_foranea;
		struct timeval timeout;
		static int identificador_mensaje;
		struct mensaje mensajes_almacenados[100];
};

#endif