#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>

int puerto = 7200;

int main(void)
{
	int ints[2];
	short shortss[2];
	long longs[2];
	float floats[2];
	double doubles[2];
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
		// RECIBIENDO INTS
		recvfrom(s, (char *) ints, 2 * sizeof(int), 0, (struct sockaddr *)&msg_to_client_addr, &clilen);
		printf("N1: %d N2: %d\n", ints[0], ints[1]);

		// RECIBIENDO SHORTS
		recvfrom(s, (char *) shortss, 2 * sizeof(short), 0, (struct sockaddr *)&msg_to_client_addr, &clilen);
		printf("N1: %d N2: %d\n", shortss[0], shortss[1]);

		// RECIBIENDO LONGS
		recvfrom(s, (char *) longs, 2 * sizeof(long), 0, (struct sockaddr *)&msg_to_client_addr, &clilen);
		printf("N1: %ld N2: %ld\n", longs[0], longs[1]);

		// RECIBIENDO FLOAT
		recvfrom(s, (char *) floats, 2 * sizeof(float), 0, (struct sockaddr *)&msg_to_client_addr, &clilen);
		printf("N1: %E N2: %E\n", floats[0], floats[1]);

		// RECIBIENDO DOUBLES
		recvfrom(s, (char *) doubles, 2 * sizeof(double), 0, (struct sockaddr *)&msg_to_client_addr, &clilen);
		printf("N1: %E N2: %E  \n", doubles[0], doubles[1]);
	}
}

