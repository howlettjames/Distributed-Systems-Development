#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <stdlib.h>

#define PUERTO_A_ESCUCHAR 8200

int main(int argc, char *argv[])
{
	int num[2];
	int s, res, clilen;
	struct sockaddr_in server_addr, msg_to_client_addr;

	// CREANDO SOCKET   
	s = socket(AF_INET, SOCK_DGRAM, 0);

	// HABILITANDO RECIBIR MENSAJES BROADCAST
	int yes = 1;
	if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, &yes, sizeof(int)) < 0)
	{
		printf("Error: No se pudo poner la opcion de Broadcast\n");
		exit(-1);
	}

	/// HABILITANDO REUSO DEL PUERTO, PARA QUE MULTIPLES PROCESOS DENTRO DE LA MISMA COMPUTADORA PUEDAN RECIBIR MENSAJES EN ESE PUERTO
	int reuse = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) == -1)
	{
		printf("Error al utilizar la opcion reuseport\n");
		exit(EXIT_FAILURE);
	}

	/* se asigna una direccion al socket del servidor*/
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(PUERTO_A_ESCUCHAR);

	// HACIENDO BIND
	if(bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
	{
		printf("Error: No se pudo hacer el bind()\n");
		exit(-1);
	}

	clilen = sizeof(msg_to_client_addr);
	printf("Esperando peticiones...\n");
	while(1) 
	{
		recvfrom(s, (char *) num, 2 * sizeof(int), 0, (struct sockaddr *)&msg_to_client_addr, &clilen);
		res = num[0] + num[1];

		printf("Res: %d\n", res);
		/* envía la petición al cliente. La estructura msg_to_client_addr contiene la dirección socket del cliente */
		sendto(s, (char *)&res, sizeof(int), 0, (struct sockaddr *)&msg_to_client_addr, clilen);
	}
}

