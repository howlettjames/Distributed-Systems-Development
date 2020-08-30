#include <iostream>
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

struct mensaje * Respuesta::getRequest(void)
{
	// RECIBIMOS LA SOLICITUD DEL CLIENTE EN UN DATAGRAMA VACÍO CON EL ESPACIO SUFICIENTE PARA ALMACENAR UN MENSAJE
	PaqueteDatagrama paquete(sizeof(struct mensaje));
	socketlocal->recibe(paquete);

	// COPIAMOS LOS DATOS DEL MENSAJE HACIA NUESTRA VARIABLE MIEMBRO "MESSAGE_RECV" Y REGRESAMOS SU DIRECCIÓN
	// LO HACEMOS DE ESTA MANERA PORQUE DE OTRA FORMA AL ACABAR LA FUNCIÓN LA MEMORIA ES LIBERADA Y LOS DATOS DESTRUIDOS
	memcpy(&message_recv, paquete.obtieneDatos(), sizeof(struct mensaje));
	
	// GUARDAMOS LA DIRECCIÓN DEL CLIENTE EN VAR MIEMBRO DE LA CLASE PARA FUTURA RESPUESTA
	// V1
	// memcpy(ip, paquete.obtieneDireccion(), sizeof(paquete.obtieneDireccion()) / sizeof(paquete.obtieneDireccion()[0])); ? No funciona, no entiendo la razón
	// V2 
	memcpy(ip, paquete.obtieneDireccion(), strlen(paquete.obtieneDireccion())); // así, sí funciona
	ip[strlen(paquete.obtieneDireccion())] = '\0';
	puerto = paquete.obtienePuerto();

	return &message_recv;
}