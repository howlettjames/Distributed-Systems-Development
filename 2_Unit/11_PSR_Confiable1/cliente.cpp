#include <iostream>
#include "Solicitud.h"

using namespace std;

// DIRECCION DEL SERVIDOR
int puerto_servidor = 7200;
// char *ip_servidor = "127.0.0.1";
char *ip_servidor = "192.168.100.61";

int main(int argc, char *argv[])
{
	// ARGUMENTOS A ENVIAR AL SERVIDOR
	int arguments[2];
	int n, i = 0;

	if(argc != 4)
	{
		cout << "Modo de uso: ./cliente <argumento1> <argumento2> <n>" << endl;
		exit(EXIT_FAILURE);
	}

	arguments[0] = atoi(argv[1]);	
	arguments[1] = atoi(argv[2]);
	n = atoi(argv[3]);

	// CREAMOS UN OBJETO SOLICITUD PARA HACER SOLICITUD DE OPERACIONES
	Solicitud solicitud;

	while(i < n)
	{
		// LLAMAMOS A LA OPERACIÓN REMOTA SUMA
		char *argumentos = solicitud.doOperation(ip_servidor, puerto_servidor, suma, (char *) arguments);

		// VARIABLE EN LA QUE RECOGEMOS LA RESPUESTA
		int *respuesta = (int *) malloc(sizeof(int));

		// COPIAMOS EL VALOR EN LA VARIABLE
		memcpy(respuesta, argumentos, sizeof(int));

		// IMPRIMIENDO EL VALOR DEVUELTO
		// cout << "Resultado de suma: " << *respuesta << endl;

		// LIBERAMOS LA MEMORIA DE LA VARIABLE DONDE SE GUARDARON LOS ARGUMENTOS RESPUESTA
		delete argumentos;
		
		i++;
	}

	return 0;
}