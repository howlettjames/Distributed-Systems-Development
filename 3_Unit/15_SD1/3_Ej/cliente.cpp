#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "SocketDatagrama.h"
#include "registro.h"	// LIBRERÍA QUE DEFINE EL FORMATO DE UN REGISTRO

using namespace std;

// #define SERVIDOR_IP "127.0.0.1"
#define SERVIDOR_IP "25.47.119.146"

#define SERVIDOR_PUERTO 7200

int main(int argc, char const *argv[])
{
	struct registro reg;
	int registros_fd, bytes_leidos;

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

	// CREAMOS UN SOCKET DESDE EL CUAL EL CLIENTE ENVIARÁ DATAGRAMAS, DESDE CUALQUIER PUERTO HENCE "0"
	SocketDatagrama socket_cliente(0);
	
	int registros_a_enviar = atoi(argv[1]);
	// LEEMOS DEL ARCHIVO FUENTE DE REGISTROS
	while((bytes_leidos = read(registros_fd, &reg, sizeof(struct registro))) > 0 && registros_a_enviar > 0)
	{
		printf("Registro leido\n");
		printf("Cel: %s\n", reg.celular);
		printf("CURP: %s\n", reg.CURP);
		printf("Partido: %s\n", reg.partido);

		// CREAMOS UN PAQUETE CON EL PUERTO Y DIRECCIÓN DEL SERVIDOR
		PaqueteDatagrama paquete((char *) &reg, sizeof(struct registro), SERVIDOR_IP, SERVIDOR_PUERTO);
		
		// ENVÍAMOS EL DATAGRAMA AL SERVIDOR
		socket_cliente.envia(paquete);

		printf("Datagrama con registro enviado al servidor.\n\n");

		registros_a_enviar--;
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