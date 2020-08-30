#include <iostream>
#include "SocketDatagrama.h"

using namespace std;

int puerto = 7200;

int main(int argc, char const *argv[])
{
	// MENSAJE A ENVIAR AL SERVIDOR
	// char *mensaje = "Hola Servidor";
	int n[2];

	n[0] = 5;
	n[1] = 3;

	// CREAMOS UN SOCKET DESDE EL CUAL EL CLIENTE ENVIARÁ DATAGRAMAS
	SocketDatagrama socket_cliente(0);
	// CREAMOS UN DATAGRAMA CON EL PUERTO Y DIRECCIÓN DEL SERVIDOR
	PaqueteDatagrama paquete((char *) n, 2 * sizeof(int), "127.0.0.1", puerto);
	// ENVÍAMOS EL DATAGRAMA AL SERVIDOR PARA QUE HAGA EL CALCULO DE SUMA
	socket_cliente.envia(paquete);

	// ESPERAMOS RECIBIR LA RESPUESTA DEL SERVIDOR
	PaqueteDatagrama paquete1(sizeof(int));
	socket_cliente.recibe(paquete1);
	// IMPRIMIMOS LA INFORMACIÓN RECIBIDA 
	int *res = (int *) paquete1.obtieneDatos();
	cout << "Resultado de suma: " << *res << endl;
	cout << "Paquete recibido desde: " << paquete1.obtieneDireccion() << " : " << paquete1.obtienePuerto() << endl;
		
	return 0;
}