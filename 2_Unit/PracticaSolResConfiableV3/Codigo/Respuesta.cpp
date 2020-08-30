#include <iostream>
#include <unistd.h>
#include "Respuesta.h"

using namespace std;

Respuesta::Respuesta(int pl)
{
	socketlocal = new SocketDatagrama(pl);
}

Respuesta::~Respuesta()
{
	delete socketlocal;
}

void Respuesta::sendReply(char *respuesta)
{
	// CREAMOS UNA ESTRUCTURA MENSAJE PARA ENVIAR LA RESPUESTA
	struct mensaje men;

	// LLENAMOS LOS CAMPOS CORRESPONDIENTES
	men.messageType = 1;
	men.requestId = message_recv.requestId;	
	men.operationId = message_recv.operationId;
	// EN ESTA PARTE ENVIAMOS EL O LOS ARGUMENTOS DE RESPUESTA OBTENIDOS DE REALIZAR LA OPERACIÓN SOLICITADA
	memcpy(men.arguments, respuesta, sizeof(respuesta) / sizeof(respuesta[0]));

	// CREAMOS UN DATAGRAMA PARA SER ENVIADO CON LA DIRECCIÓN IP Y PUERTO PREVIAMENTE GUARDADOS EN LAS VAR MIEMBRO DE LA CLASE
	PaqueteDatagrama paquete((char *) &men, sizeof(struct mensaje), ip, puerto);

	socketlocal->envia(paquete);
}

struct mensaje *Respuesta::getRequest(void)
{
	bool cliente_encontrado;

	// RECIBIMOS LA SOLICITUD DEL CLIENTE EN UN DATAGRAMA VACÍO CON EL ESPACIO SUFICIENTE PARA ALMACENAR UN MENSAJE
	PaqueteDatagrama paquete(sizeof(struct mensaje));
	socketlocal->recibe(paquete);

	// COPIAMOS LOS DATOS DEL MENSAJE HACIA NUESTRA VARIABLE MIEMBRO "MESSAGE_RECV"
	memcpy(&message_recv, paquete.obtieneDatos(), sizeof(struct mensaje));
		
	// GUARDAMOS LA DIRECCIÓN DEL IP Y EL PUERTO DEL CLIENTE QUE HACE LA SOLICITUD, PARA POSTERIORMENTE IDENTIFICARLO EN NUESTRA PEQUEÑA BD
	memcpy(ip, paquete.obtieneDireccion(), strlen(paquete.obtieneDireccion()));
	ip[strlen(paquete.obtieneDireccion())] = '\0';
	puerto = paquete.obtienePuerto();
	
	// HACEMOS LA BÚSQUEDA DEL CLIENTE EN NUESTRA BD, UN CLIENTE TIENE UN IDENTIFICADOR ÚNICO
	// FORMADO POR SU IP Y SU PUERTO
	cliente_encontrado = false;
	for(int i = 0; i < CAPACIDAD_CLIENTES; i++)
	{
		if(strcmp(clientes[i].ip, ip) == 0)
		{
			if(clientes[i].puerto == puerto)
			{
				// cout << "Cliente hallado en la BD" << endl;
				cliente_encontrado = true;
				ncliente = i;
			}
		}

		if(cliente_encontrado)
			break;
	}
	
	// SI EL CLIENTE NO FUE ENCONTRADO LO ANEXAMOS A LA BD CON SUS DATOS CORRESPONDIENTES
	if(!cliente_encontrado)
	{
		// cout << "Cliente no hallado en BD" << endl;
		// cout << "Agregando nuevo cliente: " << ncliente << endl;
		memcpy(clientes[ncliente].ip, ip, strlen(ip));
		clientes[ncliente].ip[strlen(ip)] = '\0';
		clientes[ncliente].puerto = puerto;
		clientes[ncliente].requestId = message_recv.requestId;		// Almacenamos el ID de la solicitud, esto es importante, porque
																	// así evitaremos solicitudes repetidas del mismo cliente.
	}
	else
	{
		// COMPROBAMOS SI EL ID DE SOLICITUD DEL CLIENTE RECIBIDO ES IGUAL A LA QUE YA TENIAMOS EN LA BD
		// EN CUYO CASO MODIFICAMOS EL ID DE OPERACIÓN A 0, INDICANDO AL SERVIDOR QUE NO REALICE NINGUNA OPERACIÓN
		if(message_recv.requestId == clientes[ncliente].requestId)
		{
			cout << "Esta solicitud ya ha sido hecha" << endl;
			message_recv.operationId = 0;
		}
		else // SI SE TRATA DE UNA NUEVA ID DE SOLICITUD, LA ALMACENAMOS EN DICHO CLIENTE
			clientes[ncliente].requestId = message_recv.requestId;
	}

	// ESTA FUE LA MAYOR INCERTIDUMBRE DE LA PRÁCTICA, PARA INDICARLE AL SERVIDOR A QUE CLIENTE DEBE HACER EL DEPOSITO,
	// PUES ÉSTE MANTIENE UNA PEQUEÑA BD CON LOS SALDOS DE SUS CLIENTES,
	// SE NECESITA DE ALGÚN MEDIO, ASÍ QUE DECIDIMOS INDICAR EL NÚMERO DE CLIENTE EN EL CAMPO "messageType" DEL MENSAJE,
	// TAMBIÉN SE PUDO HABER AGREGADO UN CAMPO MÁS EN EL STRUCT MENSAJE.
	message_recv.messageType = ncliente;

	return &message_recv;
}