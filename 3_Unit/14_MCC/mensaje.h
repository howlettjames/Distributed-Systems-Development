#ifndef MENSAJE_H_
#define MENSAJE_H_

#define TAM_MAX_DATA 500

// FORMATO DE CADA MENSAJE
struct mensaje
{
	int identificador_mensaje;			
	char arguments[TAM_MAX_DATA];
};

#endif