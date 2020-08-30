#include <iostream>
#include <chrono>
#include "Respuesta.h"

using namespace std;
using namespace std::chrono;

int puerto = 7200;

int fsuma(int a, int b);

int main(int argc, char const *argv[])
{
	// CREAMOS UN OBJETO RESPUESTA PARA ATENDER AL CLIENTE
	Respuesta respuesta(puerto);

	// VAR PARA ALMACENAR EL MENSAJE RECIBIDO
	struct mensaje *mensaje_recv;
	// PREPARAMOS UNA VARIABLE PARA ALMACENAR LOS DOS ARGUMENTOS PASADOS
	int *arguments = (int *) malloc(2 * sizeof(int));
	// VAR PARA ALMACENAR EL RESULTADO
	int resultado;

	double elapsed_time_ms = 0;					// var para almacenar el tiempo que lleva corriendo el servidor
	bool reset_time = true;						// var para resetear a 0 el tiempo que lleva el servidor atendiendo solicitudes, cada seg se reseteará
	time_point<high_resolution_clock> start;	// var para almacenar el momento/tiempo en que se acepto la primer conexión de cada segundo
	register int nsolicitudes = 0;				// var para almacenar el número de solicitudes por seg
	double solicitudesTotales = 0;				// var para almacenar el total de solicitudes atendidas
	double solicitudesPromedio = 0;				// var para almacenar el promedio de solicitudes atendidas
	double segs_atendiendo = 1;					// var para llevar cuenta de cada segundo
	while(1)
	{
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
		// cout << "\nSOLICITUD RECIBIDA" << endl;
		// cout << "Tipo de mensaje: " << mensaje_recv->messageType << endl;
		// cout << "ID de solicitud: " << mensaje_recv->requestId << endl;
		// cout << "ID de operación: " << mensaje_recv->operationId << endl;

		// COPIAMOS LOS ARGUMENTOS EN LA VARIABLE
		memcpy(arguments, mensaje_recv->arguments, 2 * sizeof(int));

		// EJECUTAMOS LA OPERACIÓN SOLICITADA
		resultado = 0;
		if(mensaje_recv->operationId == suma)
			resultado = fsuma(arguments[0], arguments[1]);
		
		// ENVIAMOS LA RESPUESTA
		respuesta.sendReply((char *) &resultado);

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

int fsuma(int a, int b)
{
	return a + b;
}