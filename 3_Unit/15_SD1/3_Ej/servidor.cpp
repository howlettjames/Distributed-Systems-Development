#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "SocketDatagrama.h"
#include "registro.h"

using namespace std;

#define PUERTO_A_ESCUCHAR 7200

int main(int argc, char const *argv[])
{
	int registros_fd;

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

	// CREANDO UN SOCKET EN EL PUERTO 72000 DONDE EL SERVIDOR PODRÁ ESCUCHAR CLIENTES
	SocketDatagrama socket_servidor(PUERTO_A_ESCUCHAR);
	// CREAMOS UN PAQUETE CON UN MENSAJE VACÍO QUE ACEPTA HASTA 1 REGISTRO
	PaqueteDatagrama paquete(sizeof(struct registro));

	// CREAMOS UNA VAR DE REGISTRO PARA ALMACENAR TEMPORALMENTE LOS REGISTROS RECIBIDOS
	struct registro reg_recibido;
	// EL SERVIDOR CORRE INDEFINIDAMENTE, ESCUCHANDO EN EL PUERTO 7200
	while(1)
	{
		printf("Esperando paquete...\n");

		// LLAMÁMOS A LA FUNCIÓN RECIBE, LA CUAL ALMACENARÁ UN DATAGRAMA RECIBIDO POR ALGÚN CLIENTE EN LA VAR "PAQUETE"
		socket_servidor.recibe(paquete);

		// COPIAMOS LA INFORMACIÓN BYTE A BYTE DENTRO DE LA VAR QUE CREAMOS PARA ALMACENAR UN REGISTRO TEMPORALMENTE
		memcpy(&reg_recibido, paquete.obtieneDatos(), sizeof(struct registro));

		// IMPRIMIMOS LA INFORMACIÓN DEL REMITENTE
		cout << "Paquete recibido desde: " << paquete.obtieneDireccion() << " : " << paquete.obtienePuerto() << endl;
		printf("Cel: %s\n", reg_recibido.celular);
		printf("CURP: %s\n", reg_recibido.CURP);
		printf("Partido: %s\n", reg_recibido.partido);
		
		// ESCRIBIENDO EL REGISTRO EN EL ARCHIVO INDICADO POR LÍNEA DE COMANDOS
		write(registros_fd, &reg_recibido, sizeof(struct registro));

		printf("Registro almacenado en el archivo %s\n\n", argv[1]);
	}

	return 0;
}