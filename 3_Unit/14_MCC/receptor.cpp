#include <iostream>
#include "SocketMulticast.h"

using namespace std;

#define IP_MULTICAST "224.0.0.0"
// #define IP_MULTICAST "224.0.1.0"			
// #define IP_MULTICAST "239.0.0.0"			

// #define IP_MULTICAST "25.255.255.255"
// #define IP_MULTICAST "224.255.255.255" 	// Broadcast
// #define IP_MULTICAST "25.47.119.146" 	// Esteban
// #define IP_MULTICAST "25.121.204.24" 	// Mía
#define PUERTO_ESCUCHA 7200

int main(int argc, char *argv[])
{	
	int nbd = 0;
	int cantidad_a_depositar;

	// CREANDO SOCKET MULTICAST
	SocketMulticast socket_multicast(PUERTO_ESCUCHA);

	// UNIÉNDOSE AL GRUPO MULTICAST
	socket_multicast.unirseGrupo(IP_MULTICAST);

	// CREANDO PAQUETE VACÍO PARA RECIBIR INFORMACIÓN
	PaqueteDatagrama paquete(8);

	while(1)
	{
		// ESPERANDO RECIBIR PAQUETE
		printf("Esperando recibir paquete...\n");
		int outcome = socket_multicast.recibeConfiable(paquete);

		// HACIENDO EL DEPOSITO CORRESPONDIENTE
		cantidad_a_depositar = *(int *) paquete.obtieneDatos();
		nbd += cantidad_a_depositar;

		printf("Cantidad a depositar: %d.\n", cantidad_a_depositar);
		printf("|+|+|+|Saldo en cuenta: %d\n", nbd);

		// CHECANDO SI UN MENSAJE PREVIO NO NOS LLEGÓ, EN CUYO CASO ESPERAMOS RECIBIRLO
		if(outcome == -2)
		{
			printf("Un mensaje anterior no llego, recibiendolo...\n");
			socket_multicast.recibe(paquete);
			cantidad_a_depositar = *(int *) paquete.obtieneDatos();
			nbd += cantidad_a_depositar;
			printf("Cantidad a depositar de reenvio: %d.\n", cantidad_a_depositar);
			printf("|+|+|+|Saldo en cuenta despues de reenvio: %d\n", nbd);
		}
		printf("\n");
	}
}

