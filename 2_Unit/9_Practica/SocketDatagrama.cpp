#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdio.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "SocketDatagrama.h"

using namespace std;

SocketDatagrama::SocketDatagrama(int puerto)
{
	s = socket(AF_INET, SOCK_DGRAM, 0);

	bzero((char *)&direccionLocal, sizeof(direccionLocal));
	direccionLocal.sin_family = AF_INET;
	direccionLocal.sin_addr.s_addr = INADDR_ANY;
	direccionLocal.sin_port = htons(puerto);

	if(bind(s, (struct sockaddr *)&direccionLocal, sizeof(direccionLocal)) != 0)
	{
		cout << "Error: cannot bind" << endl;
		exit(0);
	}
}

int SocketDatagrama::recibe(PaqueteDatagrama &p)
{
	unsigned int clilen = sizeof(direccionForanea);

	int nBytesRecv = recvfrom(s, p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *)&direccionForanea, &clilen);
	p.inicializaPuerto(ntohs(direccionForanea.sin_port));
	bzero(p.obtieneDireccion(), 16);
	p.inicializaDireccion(inet_ntoa(direccionForanea.sin_addr));

	return nBytesRecv;
}

int SocketDatagrama::envia(PaqueteDatagrama &p)
{
	bzero((char *)&direccionForanea, sizeof(direccionForanea));
	direccionForanea.sin_family = AF_INET;
	direccionForanea.sin_addr.s_addr = inet_addr(p.obtieneDireccion());
	direccionForanea.sin_port = htons(p.obtienePuerto());

	return sendto(s, (char *) p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *) &direccionForanea, sizeof(direccionForanea));
}

SocketDatagrama::~SocketDatagrama()
{
	close(s);
}
