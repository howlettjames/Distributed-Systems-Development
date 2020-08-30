#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "Respuesta.h"
#include "registro.h"

using namespace std;

#define PUERTO_A_ESCUCHAR 7200

int main(int argc, char const *argv[])
{
	int registros_fd;
	char mensaje_confirmacion[32] = "Registro recibido exitosamente.";

	// CHECANDO QUE LA INVOCACIÓN SEA CORRECTA
	if(argc != 2)
	{
		printf("Forma de uso: %s <nombre_archivo_guardar_registros>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// ABRIENDO ARCHVIO PARA ESCRITURA DE REGISTROS
	if((registros_fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1)
	{
		perror(argv[1]);
		exit(-1);
	}

	// CREAMOS UN OBJETO RESPUESTA PARA ATENDER AL CLIENTE
	Respuesta respuesta(PUERTO_A_ESCUCHAR);

	// VAR PARA ALMACENAR EL MENSAJE RECIBIDO
	struct mensaje *mensaje_recv;
	
	// CREAMOS UNA VAR DE REGISTRO PARA ALMACENAR TEMPORALMENTE LOS REGISTROS RECIBIDOS
	struct registro registro_recibido;
	// EL SERVIDOR CORRE INDEFINIDAMENTE, ESCUCHANDO EN EL PUERTO 7200
	while(1)
	{
		printf("Esperando solicitud...\n");

		// GUARDAMOS LA REFERENCIA AL MENSAJE RECIBIDO
		mensaje_recv = respuesta.getRequest();

		// IMPRIMIMOS LA INFORMACIÓN RECIBIDA 
		cout << "SOLICITUD RECIBIDA" << endl;
		cout << "Tipo de mensaje: " << mensaje_recv->messageType << endl;
		cout << "ID de solicitud: " << mensaje_recv->requestId << endl;
		cout << "ID de operación: " << mensaje_recv->operationId << endl;

		// COPIAMOS EL(LOS) ARGUMENTOS(REGISTRO) EN LA VAR REGISTRO_RECIBIDO
		memcpy(&registro_recibido, mensaje_recv->arguments, sizeof(struct registro));

		// EJECUTAMOS LA OPERACIÓN SOLICITADA
		if(mensaje_recv->operationId == guardar_registro)
		{
			// IMPRIMIMOS LA INFORMACIÓN DEL REMITENTE
			printf("REGISTRO\n");
			printf("Cel: %s\n", registro_recibido.celular);
			printf("CURP: %s\n", registro_recibido.CURP);
			printf("Partido: %s\n", registro_recibido.partido);
			
			// ESCRIBIENDO EL REGISTRO EN EL ARCHIVO INDICADO POR LÍNEA DE COMANDOS
			write(registros_fd, &registro_recibido, sizeof(struct registro));
			printf("Registro almacenado en el archivo: %s\n", argv[1]);

			// ENVIAMOS LA RESPUESTA
			respuesta.sendReply(mensaje_confirmacion);
			printf("Confirmacion enviada.\n\n");
		}
		else if(mensaje_recv->operationId == 0)
		{
			// SI EL ID DE OPERACIÓN ES CERO, EL SERVIDOR NO EFECTUARÁ NINGUNA OPERACIÓN, SE DESCARTA LA SOLICITUD REPETIDA
			continue;	
		} 	
	}

	return 0;
}