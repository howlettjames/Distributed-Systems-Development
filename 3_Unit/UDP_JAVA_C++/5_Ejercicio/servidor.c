#include <stdio.h>
#include <strings.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

int puerto = 7200;

struct mensaje
{
	int entero;
	float flotante;
	double doble;
	long largo;
};

int main(void)
{
	struct mensaje message;
	int s, clilen;
	struct sockaddr_in server_addr, msg_to_client_addr;
	   
	s = socket(AF_INET, SOCK_DGRAM, 0);

	/* se asigna una direccion al socket del servidor*/
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = INADDR_ANY;
	server_addr.sin_port = htons(puerto);
	bind(s, (struct sockaddr *)&server_addr, sizeof(server_addr));
	clilen = sizeof(msg_to_client_addr);

	while(1) 
	{
		char request[500];
		bzero((void *) request, 500);

		// RECIBIENDO ESTRUCTURA EN FORMA DE CADENA, DADO QUE NO JAVA NO SOPORTA ESTRUCTURAS AL ESTILO DE C/C++
		recvfrom(s, (char *) request, 500, 0, (struct sockaddr *) &msg_to_client_addr, &clilen);

		// A CONTINUACIÓN LAS VAR QUE NECESITAREMOS PARA CONVERTIR LA CADENA SEPARADA POR ESPACIOS EN
		// SUS RESPECTIVOS VALORES (ENTERO, FLOTANTE, DOBLE Y LARGO)
		int i = 0, j = 0;
		int dato = 0; 				// 0: entero, 1: flotante, 2: doble, 3: largo	
		char temp[100];
		while(dato < 4)
		{
			j = 0;
			while(request[i] != ' ')
				temp[j++] = request[i++];

			temp[j] = '\0';

			if(dato == 0)
				message.entero = atoi(temp);
			else if(dato == 1)
				message.flotante = atof(temp);
			else if(dato == 2)
				message.doble = atof(temp);
			else 
				message.largo = atol(temp);

			dato++;
			i++;
		}
		
		printf("ESTRUCTURA RECIBIDA: \n");
		printf("Entero: %d\n", message.entero);
		printf("Flotante: %E\n", message.flotante);
		printf("Doble: %E\n", message.doble);
		printf("Largo: %ld\n", message.largo);	
	}
}

