#ifndef MENSAJE_H_
#define MENSAJE_H_

// #define TAM_MAX_DATA 4000
#define TAM_MAX_DATA 16

// OPERACIONES PERMITIDAS
#define suma 1

// FORMATO DE CADA MENSAJE
struct mensaje
{
	int messageType;			// 0=Solicitud, 1=Respuesta
	unsigned int requestId;		//Identificador de mensaje
	int operationId;			//Identificador de la operacion
	char arguments[TAM_MAX_DATA];
};

#endif