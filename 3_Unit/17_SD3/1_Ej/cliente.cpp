#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "Solicitud.h"
#include "registro.h"	// LIBRERÍA QUE DEFINE EL FORMATO DE UN REGISTRO

using namespace std;

#define SERVIDOR_IP "127.0.0.1"
// #define SERVIDOR_IP "25.47.119.146"

#define SERVIDOR_PUERTO 7200

int main(int argc, char const *argv[])
{
	struct registro reg;
	int registros_fd, bytes_leidos;
	char *argumentos_respuesta;
	struct timeval timestamp;

	// CHECANDO QUE LA INVOCACIÓN SEA CORRECTA
	if(argc != 2)
	{
		printf("Forma de uso: %s <numero_registros_a_enviar>\n", argv[0]);
		exit(0);
	}

	// ABRIENDO ARCHIVO PARA LECTURA DE REGISTROS
	if((registros_fd = open("registros.dat", O_RDONLY)) == -1)
	{
		perror("registros.dat");
		exit(-1);
	}

	// CREAMOS UN OBJETO SOLICITUD PARA HACER SOLICITUD DE OPERACIONES
	Solicitud solicitud;

	int registros_a_enviar = atoi(argv[1]);
	// LEEMOS DEL ARCHIVO FUENTE DE REGISTROS
	while((bytes_leidos = read(registros_fd, &reg, sizeof(struct registro))) > 0 && registros_a_enviar > 0)
	{
		printf("Registro leido\n");
		printf("Cel: %s\n", reg.celular);
		printf("CURP: %s\n", reg.CURP);
		printf("Partido: %s\n", reg.partido);
		printf("Datagrama enviado, esperando confirmacion del servidor...\n");

		// LLAMAMOS A LA OPERACIÓN REMOTA "GUARDAR REGISTRO", LA CUAL POR AHORA, NOS DEVOLVERÁ UNA CADENA INDICANDO EL REGISTRO EXITOSO
		argumentos_respuesta = solicitud.doOperation(SERVIDOR_IP, SERVIDOR_PUERTO, guardar_registro, (char *) &reg, sizeof(struct registro));

		// SABEMOS QUE RECIBIREMOS UNA ESTRUCTURA TIMEVAL
		memcpy(&timestamp, argumentos_respuesta, sizeof(struct timeval));

		// IMPRIMIENDO MENSAJE DE CONFIRMACIÓN
		printf("Momento en que fue realizada la transaccion en el servidor: %ld : %ld\n\n", timestamp.tv_sec, timestamp.tv_usec);
			
		registros_a_enviar--;
		// LIBERAMOS LA MEMORIA DE LA VARIABLE DONDE SE GUARDARON LOS ARGUMENTOS RESPUESTA
		delete argumentos_respuesta;
	}
	
	// CHECANDO SI YA NO HAY MÁS REGISTROS O SI OCURRIÓ ALGÚN ERROR DE LECTURA
	if(bytes_leidos == 0)
	{
		printf("Ya no hay mas registros por leer. Terminando el programa...\n");
		exit(EXIT_SUCCESS);
	}
	else if(bytes_leidos < 0)
	{
		fprintf(stderr, "Ocurrio un error de lectura. Terminando el programa...\n");
		exit(EXIT_FAILURE);
	}	
	
	return 0;
}