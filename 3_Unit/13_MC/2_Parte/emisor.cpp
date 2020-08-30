#include <iostream>
#include "SocketMulticast.h"

using namespace std;

#define IP_MULTICAST "224.0.1.0"
#define PUERTO_RECEPTOR 7200

int main(int argc, char *argv[])
{
	int enteros[2];

	// CHECANDO QUE LA INVOCACIÓN HAYA SIDO CORRECTA	
	if(argc != 2)
	{
		printf("Error en la forma de uso, esta debe ser: %s <numero de receptores>\n", argv[0]);
		exit(1);
	}

	// ENVIAREMOS DOS ENTEROS PARA QUE SEAN SUMADOS
	enteros[0] = 5;
	enteros[1] = 3;

	// CREANDO SOCKET MULTICAST
	SocketMulticast socket_multicast(0);

	// CREANDO PAQUETE CON PARÁMETROS: mensaje, longitud de mensaje, dirección IP multicast, puerto receptor
	PaqueteDatagrama paquete((char *) enteros, 2 * sizeof(int), IP_MULTICAST, PUERTO_RECEPTOR);

	// ENVIANDO PAQUETE CON EL TTL INDICADO
	socket_multicast.envia(paquete, 1);

	// CREAMOS UN PAQUETE VACÍO PARA ALMACENAR LAS RESPUESTAS
	PaqueteDatagrama paquete_recepcion(8);

	int nreceptor = 0;
	int mensajes_esperados = atoi(argv[1]);
	while(mensajes_esperados > 0)
	{
		printf("Esperando respuestas...\n");

		// RECIBIENDO PAQUETE DE RESPUESTA, SI DEVUELVE ALGO MENOR A 0 OCURRIÓ UN ERROR
		if(socket_multicast.recibeTimeout(paquete_recepcion, 5, 0) < 0)
		{
			cout << "Ocurrió un error al recibir el paquete" << endl;
			exit(0);
		}

		// EXTRAYENDO RESULTADO
		int resultado = *(int *) paquete_recepcion.obtieneDatos();

		// IMPRIMIENDO RESULTADO DE SUMA E INFORMACIÓN DE QUIEN ENVIÓ
		// cout << "IP: " << paquete_recepcion.obtieneDireccion() << endl;
		// cout << "Puerto: " << paquete_recepcion.obtienePuerto() << endl;
		printf("Resultado %d devuelto por receptor numero: %d\n", resultado, nreceptor);

		nreceptor++;
		mensajes_esperados--;
	}
}

