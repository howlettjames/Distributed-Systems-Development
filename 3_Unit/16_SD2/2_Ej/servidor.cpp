#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <chrono>

#include "Respuesta.h"
#include "registro.h"

using namespace std;
using namespace std::chrono;

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
	double elapsed_time_ms = 0;					// var para almacenar el tiempo que lleva corriendo el servidor
	bool reset_time = true;						// var para resetear a 0 el tiempo que lleva el servidor atendiendo solicitudes, cada seg se reseteará
	time_point<high_resolution_clock> start;	// var para almacenar el momento/tiempo en que se acepto la primer conexión de cada segundo
	register int nsolicitudes = 0;				// var para almacenar el número de solicitudes por seg
	double solicitudesTotales = 0;				// var para almacenar el total de solicitudes atendidas
	double solicitudesPromedio = 0;				// var para almacenar el promedio de solicitudes atendidas
	double segs_atendiendo = 1;					// var para llevar cuenta de cada segundo
	while(1)
	{
		// printf("Esperando solicitud...\n");
		// CUANDO SE CUMPLE 1 SEG, SE IMPRIMEN LAS SOLICITUDES ATENDIDAS EN ESE SEGUNDO Y LAS SOLICITUDES ATENDIDAS PROMEDIO POR SEGUNDO
		// SE RESETEA LA VAR RESET_TIME PARA QUE SE EMPIECE A CONTAR UN NUEVO SEGUNDO
		// SE LLEVA CUENTA DEL NÚMERO DE SEGUNDOS QUE LLEVA ATENDIENDO EL SERVIDOR EN LA VAR "SEGS_ATENDIENDO"
		if(elapsed_time_ms > 1000)
		{
			// cout << "Solicitudes atendidas en este segundo: " << nsolicitudes << endl;
			solicitudesTotales += nsolicitudes;
			solicitudesPromedio = solicitudesTotales / segs_atendiendo;
			cout << "Solicitudes atendidas promedio por segundo: " << solicitudesPromedio << endl;
			segs_atendiendo++;
			nsolicitudes = 0;
			reset_time = true;
		}

		// GUARDAMOS LA REFERENCIA AL MENSAJE RECIBIDO
		mensaje_recv = respuesta.getRequest();

		// SI SE RESETEA EL TIEMPO, SE VUELVE A TOMAR EL MOMENTO EN QUE EMPEZÓ LA PRIMER SOLICITUD ACEPTADA EN ESE SEGUNDO EN PARTICULAR
		if(reset_time)
		{
			reset_time = false;		
			start = high_resolution_clock::now();
		}

		// IMPRIMIMOS LA INFORMACIÓN RECIBIDA 
		// cout << "SOLICITUD RECIBIDA" << endl;
		// cout << "Tipo de mensaje: " << mensaje_recv->messageType << endl;
		// cout << "ID de solicitud: " << mensaje_recv->requestId << endl;
		// cout << "ID de operación: " << mensaje_recv->operationId << endl;

		// COPIAMOS EL(LOS) ARGUMENTOS(REGISTRO) EN LA VAR REGISTRO_RECIBIDO
		memcpy(&registro_recibido, mensaje_recv->arguments, sizeof(struct registro));

		// EJECUTAMOS LA OPERACIÓN SOLICITADA
		if(mensaje_recv->operationId == guardar_registro)
		{
			// IMPRIMIMOS LA INFORMACIÓN DEL REMITENTE
			// printf("REGISTRO\n");
			// printf("Cel: %s\n", registro_recibido.celular);
			// printf("CURP: %s\n", registro_recibido.CURP);
			// printf("Partido: %s\n", registro_recibido.partido);
			
			// ESCRIBIENDO EL REGISTRO EN EL ARCHIVO INDICADO POR LÍNEA DE COMANDOS
			write(registros_fd, &registro_recibido, sizeof(struct registro));
			// printf("Registro almacenado en el archivo: %s\n", argv[1]);

			// ENVIAMOS LA RESPUESTA
			respuesta.sendReply(mensaje_confirmacion);
			// printf("Confirmacion enviada.\n\n");
		}
		else if(mensaje_recv->operationId == 0)
		{
			// SI EL ID DE OPERACIÓN ES CERO, EL SERVIDOR NO EFECTUARÁ NINGUNA OPERACIÓN, SE DESCARTA LA SOLICITUD REPETIDA
			continue;	
		} 	

		// INCREMENTAMOS EL NÚMERO DE SOLICITUDES ATENDIDAS EN ESTE SEGUNDO EN PARTICULAR
		nsolicitudes++;

		// TOMAMOS EL TIEMPO QUE LLEVA DE EJECUCIÓN EL PROGRAMA
		auto stop = high_resolution_clock::now();
		// SE ALMACENAN EL NÚMERO DE MILISEGUNDOS QUE LLEVA ATENDIENDO EL SERVIDOR
		elapsed_time_ms = duration<double, milli>(stop - start).count();
		// cout << "Tiempo: " << elapsed_time_ms << " milisegundos" << "    Solicitudes: " << nsolicitudes << endl;
	}

	return 0;
}