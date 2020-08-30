#include <iostream>
#include "SocketMulticast.h"

using namespace std;

#define IP_MULTICAST "224.0.1.0"
#define PUERTO_ESCUCHA 7200

int main(int argc, char *argv[])
{	
	// CREANDO SOCKET MULTICAST
	SocketMulticast socket_multicast(PUERTO_ESCUCHA);

	// UNIÉNDOSE AL GRUPO MULTICAST
	socket_multicast.unirseGrupo(IP_MULTICAST);

	// CREANDO PAQUETE VACÍO PARA RECIBIR INFORMACIÓN
	PaqueteDatagrama paquete(16);

	// ESPERANDO RECIBIR PAQUETE
	printf("Esperando recibir paquete...\n");
	socket_multicast.recibe(paquete);

	int *enteros = (int *) paquete.obtieneDatos();

	// IMPRIMIENDO ENTEROS RECIBIDOS E INFORMACIÓN DE QUIEN ENVIÓ
	cout << "IP: " << paquete.obtieneDireccion() << endl;
	cout << "Puerto: " << paquete.obtienePuerto() << endl;
	cout << "Enteros: " << enteros[0] << " y " << enteros[1] << endl;

	// HACIENDO LA OPERACIÓN Y MANDANDO RESULTADO
	int suma = enteros[0] + enteros[1];

	// CREANDO PAQUETE PARA ENVÍO, CON LA DIRECCIÓN IP Y PUERTO DE QUIEN NOS HA ENVIADO LOS ENTEROS
	PaqueteDatagrama paquete_envio((char *) &suma, sizeof(int), paquete.obtieneDireccion(), paquete.obtienePuerto());

	// ENVIANDO PAQUETE CON EL TTL INDICADO
	socket_multicast.envia(paquete_envio, 1);
}

