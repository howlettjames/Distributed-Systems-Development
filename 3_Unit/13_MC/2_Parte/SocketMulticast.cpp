#include "SocketMulticast.h"

SocketMulticast::SocketMulticast(int puerto)
{
	// CREAMOS EL SOCKET PARA USO CON DATAGRAMAS Y REDES IPV4   
	s = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

	// ASIGNAMOS LA DIRECCIÓN Y PUERTO DE ESTE SOCKET
	bzero((char *) &direccion_local, sizeof(direccion_local));
	direccion_local.sin_family = AF_INET;
	direccion_local.sin_addr.s_addr = INADDR_ANY;
	direccion_local.sin_port = htons(puerto);

	// AÑADIENDO LA OPCIÓN DE SOCKET PARA PODER REUSAR EL PUERTO Y ASÍ DOS PROCESOS EN UN MISMO EQUIPO
	// PUEDAN ESCUCHAR EN UN GRUPO MULTICAST DESDE EL MISMO PUERTO.
	int reuse = 1;
	if(setsockopt(s, SOL_SOCKET, SO_REUSEPORT, &reuse, sizeof(reuse)) < 0)
	{
		perror("Error: No se pudo llamar la opcion de socket para reusar un puerto.\n");
		exit(0);
	}
	
	// ATAMOS ESTE SOCKET A DICHA DIRECCIÓN Y PUERTO
	if(bind(s, (struct sockaddr *) &direccion_local, sizeof(direccion_local)) < 0)
	{
		perror("Error: No se pudo hacer bind");
		exit(1);	
	}
}

SocketMulticast::~SocketMulticast()
{
	close(s);
}

int SocketMulticast::recibeTimeout(PaqueteDatagrama &p, time_t segundos, suseconds_t microsegundos)
{	
	unsigned int clilen = sizeof(direccion_foranea);

	// RELLENAMOS LA ESTRUCTURA UTILIZADA POR SETSOCKOPT CON LOS SEGUNDOS Y MICROSEGUNDOS SOLICITADOS
	timeout.tv_sec = segundos;
	timeout.tv_usec = microsegundos;

	// AÑADIMOS LA OPCIÓN DE TIMEOUT PARA QUE EL SOCKET ESPERE UNA RESPUESTA SOLO POR EL TIEMPO SOLICITADO
	if(setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0)
	{
		perror("Error: No se pudo asignar la opción de socket timeout");
		exit(0);
	}

	// RECIBIENDO LA DATAGRAMA, USANDO LA INFORMACIÓN PROPORCIONADA POR EL PAQUETE "P"
	int nBytesRecv = recvfrom(s, p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *) &direccion_foranea, &clilen);
	
	// COMPROBAMOS LOS BYTES LEÍDOS, SI EL VALOR ES MENOR A 0 OCURRIÓ UN PROBLEMA
	if(nBytesRecv < 0)
	{
		if(errno == EWOULDBLOCK)
			fprintf(stderr, "Tiempo de recepción transcurrido\n");
		else
			fprintf(stderr, "Error en recvfrom\n");
	}
	else
	{
		// CAPTURANDO LA DIRECCIÓN IP Y PUERTO DE QUIEN ENVIÓ EL DATAGRAMA, PARA POSTERIOR RESPUESTA
		bzero(p.obtieneDireccion(), 16);
		p.inicializaDireccion(inet_ntoa(direccion_foranea.sin_addr));
		p.inicializaPuerto(ntohs(direccion_foranea.sin_port));
	}

	return nBytesRecv;
}

int SocketMulticast::recibe(PaqueteDatagrama &p)
{
	unsigned int clilen = sizeof(direccion_foranea);

	// RECIBIENDO LA DATAGRAMA, USANDO LA INFORMACIÓN PROPORCIONADA POR EL PAQUETE "P"
	int nBytesRecv = recvfrom(s, p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *) &direccion_foranea, &clilen);
	// CAPTURANDO LA DIRECCIÓN IP Y PUERTO DE QUIEN ENVIÓ EL DATAGRAMA, PARA POSTERIOR RESPUESTA
	bzero(p.obtieneDireccion(), 16);
	p.inicializaDireccion(inet_ntoa(direccion_foranea.sin_addr));
	p.inicializaPuerto(ntohs(direccion_foranea.sin_port));
	
	return nBytesRecv;
}

int SocketMulticast::envia(PaqueteDatagrama &p, unsigned char ttl)
{
	// ASIGNAMOS LA OPCIÓN DE TTL(TIME TO LIVE) DEL DATAGRAMA QUE ENVIAREMOS
	if(setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &ttl, sizeof(ttl)) < 0)
	{
		perror("Error: No se pudo asignar la opción de socket multicast");
		exit(1);	
	}

	// LLENANDO LA INFORMACIÓN NECESARIA PARA ENVIAR EL DATAGRAMA, LOS DATOS LOS OBTENEMOS DEL PAQUETE QUE NOS HAN 
	// PASADO POR REFERENCIA, DICHO PAQUETE CONTIENE LA INFO DADO QUE EN LA FUNCIÓN RECIBE FUERON LLENADOS LOS CAMPOS
	// DE IP Y PUERTO DE QUIEN NOS HA ENVIADO UN MENSAJE.
	bzero((char *) &direccion_foranea, sizeof(direccion_foranea));
	direccion_foranea.sin_family = AF_INET;
	if(inet_pton(AF_INET, p.obtieneDireccion(), &direccion_foranea.sin_addr) <= 0)
	{
		perror("Error: No se pudo hacer la conversión de la IP a formato de red");
		exit(1);
	}
	direccion_foranea.sin_port = htons(p.obtienePuerto());

	// ENVIANDO DATAGRAMA
	return sendto(s, (char *) p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *) &direccion_foranea, sizeof(direccion_foranea));
}

void SocketMulticast::unirseGrupo(char *ip_multicast)
{
	struct ip_mreq multicast;

	// RELLENANDO EL STRUCT NECESARIO PARA ASIGNAR LA OPCIÓN DE SOCKET
	multicast.imr_multiaddr.s_addr = inet_addr(ip_multicast);
	multicast.imr_interface.s_addr = htonl(INADDR_ANY);

	// ASIGNANDO LA OPCIÓN DE SOCKET PARA UNIRNOS A UN GRUPO DE RECEPTORES CON LA IP PASADA A LA FUNCIÓN
	if(setsockopt(s, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void *) &multicast, sizeof(multicast)) < 0)
	{
		perror("Error: No se pudo agregar este proceso al grupo de receptores");
		exit(1);		
	}
}

void SocketMulticast::salirseGrupo(char *ip_multicast)
{
	struct ip_mreq multicast;

	// RELLENANDO EL STRUCT NECESARIO PARA ASIGNAR LA OPCIÓN DE SOCKET
	multicast.imr_multiaddr.s_addr = inet_addr(ip_multicast);
	multicast.imr_interface.s_addr = htonl(INADDR_ANY);

	// ASIGNANDO LA OPCIÓN DE SOCKET PARA SALIRNOS DE UN GRUPO DE RECEPTORES CON LA IP PASADA A LA FUNCIÓN
	if(setsockopt(s, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void *) &multicast, sizeof(multicast)) < 0)
	{
		perror("Error: No se pudo quitar a este proceso al grupo de receptores");
		exit(1);		
	}
}