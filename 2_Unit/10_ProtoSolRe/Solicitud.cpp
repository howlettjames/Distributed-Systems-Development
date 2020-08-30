#include <iostream>
#include "Solicitud.h"

using namespace std;

unsigned int Solicitud::requestId = 0;

Solicitud::Solicitud()
{
	socketlocal = new SocketDatagrama(0);
}

Solicitud::~Solicitud()
{
	delete socketlocal;
} 

char * Solicitud::doOperation(char *ip, int puerto, int operationId, char *arguments)
{
	struct mensaje men;

	// MENSAJE A ENVIAR AL SERVIDOR
	men.messageType = 0;															// 0 = Solicitud, 1 = Respuesta
	men.requestId = requestId++;													// ID de solicitud generado automáticamente
	men.operationId = operationId;													// ID de la operación deseada
	memcpy(men.arguments, arguments, sizeof(arguments) / sizeof(arguments[0]));		// Copiamos los argumentos pasados dentro del mensaje a enviar

	// CREAMOS UN DATAGRAMA CON EL PUERTO, DIRECCIÓN DEL SERVIDOR Y  EL MENSAJE A ENVIAR
	PaqueteDatagrama paquete((char *) &men, sizeof(struct mensaje), ip, puerto);

	// ENVÍAMOS EL DATAGRAMA AL SERVIDOR PARA QUE HAGA LA OPERACIÓN CORRESPONDIENTE
	socketlocal->envia(paquete);

	// ESPERAMOS RECIBIR LA RESPUESTA DEL SERVIDOR
	PaqueteDatagrama paquete1(sizeof(struct mensaje));
	socketlocal->recibe(paquete1);

	// OBTENEMOS EL MENSAJE ENVIADO POR EL SERVIDOR 
	struct mensaje *men_res = (struct mensaje *) paquete1.obtieneDatos();
	// cout << "Paquete recibido desde: " << paquete1.obtieneDireccion() << " : " << paquete1.obtienePuerto() << endl;

	// VARIABLE PARA ALMACENAR LOS ARGUMENTOS RECIBIDOS
	char *argumentos_respuesta = (char *) malloc(TAM_MAX_DATA);
	// bzero((char *) &argumentos_respuesta, sizeof(respuesta));

	// COPIAMOS LOS ARGUMENTOS EN LA VARIABLE
	memcpy(argumentos_respuesta, men_res->arguments, sizeof(men_res->arguments));

	return argumentos_respuesta;
}
