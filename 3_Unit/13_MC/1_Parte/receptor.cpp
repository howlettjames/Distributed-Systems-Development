#include <iostream>
#include "SocketMulticast.h"

using namespace std;

int main(int argc, char *argv[])
{	
	// CHECANDO QUE LA INVOCACIÓN HAYA SIDO CORRECTA	
	if(argc != 3)
	{
		printf("Error en la forma de uso, esta debe ser: %s <ip multicast> <puerto en el que escucha>\n", argv[0]);
		exit(1);
	}

	// OBTENIENDO EL PUERTO
	int puerto_escucha = atoi(argv[2]);

	// CREANDO SOCKET MULTICAST
	SocketMulticast socket_multicast(puerto_escucha);

	// UNIÉNDOSE AL GRUPO MULTICAST
	socket_multicast.unirseGrupo(argv[1]);

	// CREANDO PAQUETE VACÍO PARA RECIBIR INFORMACIÓN
	PaqueteDatagrama paquete(100);

	// ESPERANDO RECIBIR PAQUETE
	socket_multicast.recibe(paquete);

	// IMPRIMIENDO MENSAJE E INFORMACIÓN DE QUIEN ENVÍA
	cout << "IP: " << paquete.obtieneDireccion() << endl;
	cout << "Puerto: " << paquete.obtienePuerto() << endl;
	cout << "Mensaje: " << paquete.obtieneDatos() << endl;
}

