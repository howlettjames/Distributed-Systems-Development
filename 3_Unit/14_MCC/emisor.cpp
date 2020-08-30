#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "SocketMulticast.h"

using namespace std;

// #define IP_MULTICAST "224.0.0.0"			
#define IP_MULTICAST "239.0.0.0"			
// #define IP_MULTICAST "239.0.0.0"			

// #define IP_MULTICAST "25.255.255.255"
// #define IP_MULTICAST "224.255.255.255" 	// Broadcast
// #define IP_MULTICAST "25.47.119.146" 	// Esteban
// #define IP_MULTICAST "25.121.204.24" 	// Mía
#define PUERTO_RECEPTOR 7200

int main(int argc, char *argv[])
{
	int cantidad_a_depositar = 0;
	int depositos = 0;
	int saldo = 0;

	if(argc != 2)
	{
		fprintf(stderr, "Invocacion incorrecta, se debe llamar así: %s <cantidad de depositos>\n", argv[0]);
		exit(0);
	}

	srand((unsigned) time(NULL));

	// OBTENIENDO EL NÚMERO DE DEPÓSITOS
	depositos = atoi(argv[1]);

	// CREANDO SOCKET MULTICAST
	SocketMulticast socket_multicast(0);

	while(depositos > 0)
	{
		cantidad_a_depositar = (rand() % 9) + 1;
		saldo += cantidad_a_depositar;

		printf("Cantidad a depositar: %d\n", cantidad_a_depositar);
		printf("|+|+|+|Saldo en cuenta local: %d\n", saldo);

		// CREANDO PAQUETE CON PARÁMETROS: mensaje, longitud de mensaje, dirección IP multicast, puerto receptor
		PaqueteDatagrama paquete((char *) &cantidad_a_depositar, sizeof(int), IP_MULTICAST, PUERTO_RECEPTOR);

		// ENVIANDO PAQUETE CON EL TTL INDICADO Y EL NÚMERO DE RECEPTORES
		printf("Enviando mensaje multicast...\n");
		if(socket_multicast.enviaConfiable(paquete, 255, 2) < 0)
			printf("|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|+|No todos los receptores recibieron el mensaje.\n");
		else
			printf("Todos los receptores recibieron el mensaje\n"); 

		printf("\n");
		depositos--;
		// sleep(1);
	}

	/*// CREAMOS UN PAQUETE VACÍO PARA ALMACENAR LAS RESPUESTAS
	PaqueteDatagrama paquete_recepcion(8);

	int mensajes_esperados = 2;
	while(mensajes_esperados > 0)
	{
		printf("Esperando respuestas...\n");

		// RECIBIENDO PAQUETE DE RESPUESTA, SI DEVUELVE ALGO MENOR A 0 OCURRIÓ UN ERROR
		if(socket_multicast.recibeTimeout(paquete_recepcion, 2, 0) < 0)
		{
			cout << "Ocurrió un error al recibir el paquete" << endl;
			exit(0);
		}

		// EXTRAYENDO RESULTADO
		int resultado = *(int *) paquete_recepcion.obtieneDatos();

		// IMPRIMIENDO RESULTADO DE SUMA E INFORMACIÓN DE QUIEN ENVIÓ
		cout << "IP: " << paquete_recepcion.obtieneDireccion() << endl;
		cout << "Puerto: " << paquete_recepcion.obtienePuerto() << endl;
		cout << "Resultado: " << resultado << endl;

		mensajes_esperados--;
	}*/
}

