#include <iostream>
#include "SocketDatagrama.h"

using namespace std;

int puerto = 7200;

int main(int argc, char const *argv[])
{
	// CREANDO UN SOCKET EN EL PUERTO 72000 DONDE EL SERVIDOR PODRÁ ESCUCHAR CLIENTES
	SocketDatagrama socket_servidor(puerto);
	// CREAMOS UN PAQUETE CON UN MENSAJE VACÍO QUE ACEPTA HASTA 13 BYTES
	PaqueteDatagrama paquete(2 * sizeof(int));

	// EL SERVIDOR CORRE INDEFINIDAMENTE, ESCUCHANDO EN EL PUERTO 7200
	while(1)
	{
		// LLAMÁMOS A LA FUNCIÓN RECIBE, LA CUAL ALMACENARÁ UN DATAGRAMA RECIBIDO POR ALGÚN CLIENTE EN LA VAR "PAQUETE"
		socket_servidor.recibe(paquete);
		// HACEMOS UN CÁLCULO CON LOS DATOS RECIBIDOS DESDE EL CLIENTE
		int *n = (int *) paquete.obtieneDatos();
		int res = n[0] + n[1];
		// IMPRIMIMOS LA INFORMACIÓN DEL REMITENTE
		cout << "Paquete recibido desde: " << paquete.obtieneDireccion() << " : " << paquete.obtienePuerto() << endl;
		// RESPONDEMOS AL CLIENTE CON EL CALCULO YA HECHO
		PaqueteDatagrama paquete1((char *) &res, sizeof(int), paquete.obtieneDireccion(), paquete.obtienePuerto());
		socket_servidor.envia(paquete1);
	}

	return 0;
}