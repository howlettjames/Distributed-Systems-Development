#include <iostream>
#include "SocketMulticast.h"

using namespace std;

int main(int argc, char *argv[])
{
	// CHECANDO QUE LA INVOCACIÓN HAYA SIDO CORRECTA	
	if(argc != 5)
	{
		printf("Error en la forma de uso, esta debe ser: %s <ip multicast> <puerto del receptor> <ttl> <cadena a enviar>\n", argv[0]);
		exit(1);
	}

	// OBTENIENDO EL PUERTO
	int puerto_receptor = atoi(argv[2]);

	// OBTENIENDO TTL
	int ttl = atoi(argv[3]);

	// CREANDO SOCKET MULTICAST
	SocketMulticast socket_multicast(0);

	// CREANDO PAQUETE CON PARÁMETROS: mensaje, longitud de mensaje, dirección IP multicast, puerto receptor
	PaqueteDatagrama paquete(argv[4], strlen(argv[4]), argv[1], puerto_receptor);

	// ENVIANDO PAQUETE CON EL TTL INDICADO
	socket_multicast.envia(paquete, ttl);
}

