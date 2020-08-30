#include <iostream>
#include "Solicitud.h"

using namespace std;

// DIRECCION DEL SERVIDOR
int puerto_servidor = 7200;
// char *ip_servidor = "127.0.0.1";
// char *ip_servidor = "192.168.100.61";
// char *ip_servidor = "25.121.204.24";
char *ip_servidor = "25.47.119.146";

int main(int argc, char *argv[])
{
	int nDepositos, cantidad, saldo = 0;
	int respuesta;
	char *argumentos;
	int i = 0;

	srand((unsigned) time(NULL));

	if(argc != 2)
	{
		cout << "Modo de uso: ./cliente <numero de depositos>" << endl;
		exit(EXIT_FAILURE);
	}

	// NÚMERO DE DEPÓSITOS A EJECUTAR
	nDepositos = atoi(argv[1]);

	// CREAMOS UN OBJETO SOLICITUD PARA HACER SOLICITUD DE OPERACIONES
	Solicitud solicitud;

	// HACEMOS N DEPOSITOS
	while(i < nDepositos)
	{
		// LA CANTIDAD SERÁ ALEATORIA ENTRE 1 Y 9
		cantidad = (rand() % 9) + 1;
		cout << "\nCantidad a depositar en servidor: " << cantidad << endl;
		saldo += cantidad;
		cout << "Saldo: " << saldo << endl;

		// LLAMAMOS A LA OPERACIÓN REMOTA DEPÓSITO
		argumentos = solicitud.doOperation(ip_servidor, puerto_servidor, deposito, (char *) &cantidad);

		// COPIAMOS EL(LOS) ARGUMENTOS DE RESPUESTA DEL SERVIDOR EN LA VAR RESPUESTA
		memcpy(&respuesta, argumentos, sizeof(int));

		// IMPRIMIENDO EL VALOR DEVUELTO, EN ESTE CASO EL SALDO EN LA CUENTA CLIENTE QUE TIENE REGISTRADO EL SERVIDOR
		cout << "Saldo en cuenta: " << respuesta << endl;

		if(saldo == respuesta)
			cout << "Saldo OK" << endl;
		if(saldo != respuesta)
		{
			cout << "Saldo no OK" << endl;
			exit(EXIT_FAILURE);
		}

		// LIBERAMOS LA MEMORIA DE LA VARIABLE DONDE SE GUARDARON LOS ARGUMENTOS RESPUESTA
		delete argumentos;
		
		i++;
	}

	return 0;
}