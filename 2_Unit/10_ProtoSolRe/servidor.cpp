#include <iostream>
#include "Respuesta.h"

using namespace std;

int puerto = 7200;

int fsuma(int a, int b);

int main(int argc, char const *argv[])
{
	// CREAMOS UN OBJETO RESPUESTA PARA ATENDER AL CLIENTE
	Respuesta respuesta(puerto);

	// VAR PARA ALMACENAR EL MENSAJE RECIBIDO
	struct mensaje *mensaje_recv;
	// PREPARAMOS UNA VARIABLE PARA ALMACENAR LOS DOS ARGUMENTOS PASADOS
	int *arguments = (int *) malloc(2 * sizeof(int));
	int resultado;
	while(1)
	{		
		// GUARDAMOS LA REFERENCIA AL MENSAJE RECIBIDO
		mensaje_recv = respuesta.getRequest();
		
		// IMPRIMIMOS LA INFORMACIÓN RECIBIDA
		cout << "\nSOLICITUD RECIBIDA" << endl;
		cout << "Tipo de mensaje: " << mensaje_recv->messageType << endl;
		cout << "ID de solicitud: " << mensaje_recv->requestId << endl;
		cout << "ID de operación: " << mensaje_recv->operationId << endl;

		// COPIAMOS LOS ARGUMENTOS EN LA VARIABLE
		memcpy(arguments, mensaje_recv->arguments, 2 * sizeof(int));

		// EJECUTAMOS LA OPERACIÓN SOLICITADA
		resultado = 0;
		if(mensaje_recv->operationId == suma)
			resultado = fsuma(arguments[0], arguments[1]);
		
		// ENVIAMOS LA RESPUESTA
		respuesta.sendReply((char *) &resultado);
	}

	return 0;
}

int fsuma(int a, int b)
{
	return a + b;
}