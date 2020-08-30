#include <iostream>
#include "Respuesta.h"

using namespace std;

int puerto = 7200;
int nbd[10] = {0};

int fdeposito(int cantidad, int ncliente);

int main(int argc, char const *argv[])
{
	// CREAMOS UN OBJETO RESPUESTA PARA ATENDER AL CLIENTE
	Respuesta respuesta(puerto);

	// VAR PARA ALMACENAR EL MENSAJE RECIBIDO
	struct mensaje *mensaje_recv;
	// PREPARAMOS UNA VARIABLE PARA ALMACENAR EL ARGUMENTO PASADO (CANTIDAD DE DEPÓSITO)
	int cantidad_deposito;
	// VAR PARA ALMACENAR EL RESULTADO DE HACER LA OPERACIÓN, EN ESTE CASO EL SALDO EN LA CUENTA DEL CLIENTE
	int resultado_nbd;

	while(1)
	{
		// GUARDAMOS LA REFERENCIA AL MENSAJE RECIBIDO
		mensaje_recv = respuesta.getRequest();

		// IMPRIMIMOS LA INFORMACIÓN RECIBIDA 
		cout << "\nSOLICITUD RECIBIDA" << endl;
		cout << "Tipo de mensaje: " << mensaje_recv->messageType << endl;
		cout << "ID de solicitud: " << mensaje_recv->requestId << endl;
		cout << "ID de operación: " << mensaje_recv->operationId << endl;

		// COPIAMOS EL(LOS) ARGUMENTOS(CANTIDAD A DEPOSITAR) EN LA VAR CANTIDAD_DEPOSITO
		memcpy(&cantidad_deposito, mensaje_recv->arguments, sizeof(int));

		// EJECUTAMOS LA OPERACIÓN SOLICITADA
		resultado_nbd = 0;
		if(mensaje_recv->operationId == deposito)
			resultado_nbd = fdeposito(cantidad_deposito, mensaje_recv->messageType);
		else if(mensaje_recv->operationId == 0)
		{
			// SI EL ID DE OPERACIÓN ES CERO, EL SERVIDOR NO EFECTUARÁ NINGUNA OPERACIÓN, SE DESCARTA LA SOLICITUD REPETIDA
			continue;	
		} 	

		// ENVIAMOS LA RESPUESTA
		respuesta.sendReply((char *) &resultado_nbd);
	}
	
	return 0;
}

int fdeposito(int cantidad, int ncliente)
{
	nbd[ncliente] += cantidad;

	return nbd[ncliente];
}