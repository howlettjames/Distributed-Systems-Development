#include <iostream>
#include <thread>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "SocketDatagrama.h"

using namespace std;

#define SERVIDOR_PUERTO 8200

int main(int argc, char const *argv[])
{
	SocketDatagrama socket_datagrama(0);
	socket_datagrama.setBroadcast();

	int numeros[2];
	numeros[0] = 5;
	numeros[1] = 6;
	int resultado = 0;
	// char ip_broadcast[] = "255.255.255.255";
	char ip_broadcast[] = "25.255.255.255";

	PaqueteDatagrama paquete_broadcast((char *) numeros, sizeof(int) * 2, ip_broadcast, SERVIDOR_PUERTO);
	PaqueteDatagrama paquete_recepcion(sizeof(int));

	socket_datagrama.envia(paquete_broadcast);

	printf("Esperando respuesta(s) al broadcast...\n");
	while(1)
	{
		socket_datagrama.recibe(paquete_recepcion);
		printf("IP: %s\n", paquete_recepcion.obtieneDireccion());
		printf("Puerto: %d\n", paquete_recepcion.obtienePuerto());
		resultado = *(int *) paquete_recepcion.obtieneDatos();
		printf("Resultado: %d\n\n", resultado);
	}

	return 0;
}