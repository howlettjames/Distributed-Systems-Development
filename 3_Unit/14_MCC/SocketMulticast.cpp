#include "SocketMulticast.h"

int SocketMulticast::identificador_mensaje = 0;

int SocketMulticast::getIdentificadorMensaje()
{
	return identificador_mensaje;
}

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
	/*if(bind(s, (struct sockaddr *) &direccion_local, sizeof(direccion_local)) < 0)
	{
		perror("Error: No se pudo hacer bind");
		exit(1);	
	}*/
	bind(s, (struct sockaddr *) &direccion_local, sizeof(direccion_local));
}

SocketMulticast::~SocketMulticast()
{
	close(s);
}

int SocketMulticast::recibeConfiable(PaqueteDatagrama &p)
{
	unsigned int clilen = sizeof(direccion_foranea);

	// RECIBIENDO LA DATAGRAMA, ALMACENANDO LA INFORMACIÓN DENTRO DEL PAQUETEDATAGRAMA PASADO A ESTE MÉTODO
	int nBytesRecv = recvfrom(s, p.obtieneDatos(), p.obtieneLongitud(), 0, (struct sockaddr *) &direccion_foranea, &clilen);

	// CAPTURANDO LA DIRECCIÓN IP Y PUERTO DE QUIEN ENVIÓ EL DATAGRAMA, PARA POSTERIOR RESPUESTA
	bzero(p.obtieneDireccion(), 16);
	p.inicializaDireccion(inet_ntoa(direccion_foranea.sin_addr));
	p.inicializaPuerto(ntohs(direccion_foranea.sin_port));
	
	// VAR MENSAJE VACÍA PARA ALMACENAR EL MENSAJE RECIBIDO	
	struct mensaje mensaje_recibido;

	// COPIANDO LOS DATOS RECIBIDOS DENTRO DEL MENSAJE VACÍO
	memcpy(&mensaje_recibido, p.obtieneDatos(), p.obtieneLongitud());

	printf("LLego mensaje con identificador: %d\n", mensaje_recibido.identificador_mensaje);
	printf("Contenido del mensaje: %d\n", *(int *)mensaje_recibido.arguments);

	// METEMOS LOS DATOS DENTRO DEL PAQUETE QUE ESPERA MANEJAR EL USUARIO "p"
	memcpy(p.obtieneDatos(), mensaje_recibido.arguments, sizeof(mensaje_recibido.arguments) / sizeof(mensaje_recibido.arguments[0]));
	p.inicializaLongitud(sizeof(mensaje_recibido.arguments) / sizeof(mensaje_recibido.arguments[0]));

	// COMPROBAMOS QUE EL IDENTIFICADOR DE MENSAJE VAYA A LA PAR DEL QUE TENEMOS LOCALMENTE
	if(mensaje_recibido.identificador_mensaje == identificador_mensaje)
	{
		printf("Recibimos mensaje correctamente con identificador: %d\n", identificador_mensaje);
		
		// PREPARAMOS UN MENSAJE DE ACUSE DICIENDO QUE TODO ESTA OK
		struct mensaje mensaje_acuse;
		mensaje_acuse.identificador_mensaje = identificador_mensaje;
		memcpy(mensaje_acuse.arguments, "Acknowledge", 11);
		PaqueteDatagrama paquete_acuse((char *) &mensaje_acuse, sizeof(mensaje_acuse), p.obtieneDireccion(), p.obtienePuerto());
		// LO ENVIAMOS
		this->envia(paquete_acuse, 1);

		identificador_mensaje++;
	}
	else if(mensaje_recibido.identificador_mensaje > identificador_mensaje)
	{
		printf("No recibimos algun mensaje previo, enviando peticion de reenvio...\n");
		// SI EL IDENTIFICADOR DE MENSAJE DEL MENSAJE RECIBIDO ES DIFERENTE AL QUE TENEMOS, OCURRIÓ UN PROBLEMA Y NO
		// RECIBIMOS UN MENSAJE PREVIO, ASÍ QUE LO SOLICITAMOS Y REGRESAMOS AL USUARIO EL CÓDIGO "-2", QUE SIGNIFICA QUE
		// DEBERÁ RECIBIR ESTE MENSAJE POSTERIORMENTE. MIENTRAS TANTO A TRAVÉS DE "p" EL USUARIO SEGUIRÁ OBTENIENDO EL 
		// ÚLTIMO PAQUETE TRANSMITIDO. COMO SE TRATA DE UNA SUMA, NO IMPORTA EL ORDEN EN QUE LLEGUE EL MENSAJE
		struct mensaje mensaje_acuse;
		mensaje_acuse.identificador_mensaje = identificador_mensaje;
		char *men = "Reenviar\0";
		memcpy(mensaje_acuse.arguments, men, 9);
		PaqueteDatagrama paquete_acuse((char *) &mensaje_acuse, sizeof(mensaje_acuse), p.obtieneDireccion(), p.obtienePuerto());
		this->envia(paquete_acuse, 1);

		identificador_mensaje = mensaje_recibido.identificador_mensaje + 1;

		printf("Peticion de reenvio enviada.\n");
		// CÓDIGO QUE LE INDICA AL USUARIO PEDIR UN PAQUETE PÉRDIDO
		nBytesRecv = -2;
	}
	
	return nBytesRecv;
}

int SocketMulticast::enviaConfiable(PaqueteDatagrama &p, int ttl, unsigned char num_receptores)
{
	// EL PAQUETE PASADO POR REFERENCIA "p" NO ES EL QUE EN REALIDAD ENVIAREMOS, SINO QUE ENVIAREMOS LA INFORMACIÓN QUE 
	// DESEE ENVIAR EL USUARIO DENTRO DE UNA ESTRUCTURA DE TIPO "MENSAJE", ESTA ESTRUCTURA CUENTA CON
	// DOS CAMPOS, LA INFORMACIÓN Y UN IDENTIFICADOR DE MENSAJE, PARA QUE LOS RECEPTORES DEL MENSAJE COMPRUEBEN QUE ESTAN
	// ACTUALIZADOS CON EL MENSAJE QUE LES LLEGÓ. TODO ESTO ES TRANSPARENTE PARA EL USUARIO.

	// CREAMOS UNA ESTRUCTURA MENSAJE PARA ENVÍO EN LA CUAL VAMOS A ENVOLVER LA INFORMACIÓN PASADA DENTRO DEL PAQUETE "p"
	struct mensaje message_to_send;

	// LLENAMOS LOS CAMPOS CORRESPONDIENTES
	message_to_send.identificador_mensaje = identificador_mensaje;
	memcpy(message_to_send.arguments, p.obtieneDatos(), p.obtieneLongitud());

	// ASIGNAMOS LA OPCIÓN DE TTL(TIME TO LIVE) DEL DATAGRAMA QUE ENVIAREMOS
	if(setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (void *) &ttl, sizeof(ttl)) < 0)
	{
		perror("Error: No se pudo asignar la opción de socket multicast");
		exit(1);
	}	

	// LLENANDO LA INFORMACIÓN NECESARIA PARA ENVIAR EL DATAGRAMA, LOS DATOS LOS OBTENEMOS DEL PAQUETE QUE NOS HAN 
	// PASADO POR REFERENCIA.
	bzero((char *) &direccion_foranea, sizeof(direccion_foranea));
	direccion_foranea.sin_family = AF_INET;
	if(inet_pton(AF_INET, p.obtieneDireccion(), &direccion_foranea.sin_addr) <= 0)
	{
		perror("Error: No se pudo hacer la conversión de la IP a formato de red");
		exit(1);
	}
	direccion_foranea.sin_port = htons(p.obtienePuerto());

	// ENVIANDO DATAGRAMA
	sendto(s, (char *) &message_to_send, sizeof(struct mensaje), 0, (struct sockaddr *) &direccion_foranea, sizeof(direccion_foranea));

	// GUARDANDO ESTE MENSAJE EN LA BASE DE DATOS LOCAL DE MENSAJES, POR SI UN RECEPTOR NO RECIBIÓ ALGÚN MENSAJE
	mensajes_almacenados[identificador_mensaje].identificador_mensaje = identificador_mensaje;
	memcpy(mensajes_almacenados[identificador_mensaje].arguments, p.obtieneDatos(), p.obtieneLongitud());

	// CREAMOS UN PAQUETE PARA RECIBIR LOS ACUSES DE RECIBO
	PaqueteDatagrama paquete_recepcion(100);
	// CREAMOS UN MENSAJE PARA ALMACENAR LOS ACUSES
	struct mensaje ack_message;
	// VARIABLE QUE DEVOLVEREMOS AL USUARIO, SI ES NEGATIVA SIGNIFICA QUE NO LLEGÓ EL MENSAJE A TODOS LOS RECEPTORES
	int everyReceiverGotMessage = 1;

	// MANEJAMOS LA RECEPCIÓN DE ACUSES
	while(num_receptores > 0)
	{
		num_receptores--;

		// ESPERAMOS RECIBIR UN ACUSE DE RECIBO DENTRO DEL TIEMPO INDICADO, EN ESTE CASO 5 SEG
		if(this->recibeTimeout(paquete_recepcion, 5, 0) < 0)
		{
			fprintf(stderr, "Error: Un receptor no respondio con su acuse.\n");
			everyReceiverGotMessage = -1;
		}
		else
		{
			// DE HABER LLEGADO ALGÚN MENSAJE, CHECAMOS QUE SEA UN ACUSE DE RECIBO CON EL ÚLTIMO IDENTIFICADOR DE MENSAJE
			memcpy(&ack_message, paquete_recepcion.obtieneDatos(), sizeof(struct mensaje));

			printf("Acuse de recibo recibido por parte de alguno de los receptores con identificador %d\n", ack_message.identificador_mensaje);
			printf("Mensaje del receptor: %s\n", ack_message.arguments);
			if(ack_message.identificador_mensaje == identificador_mensaje)
			{
				if(strcmp(ack_message.arguments, "Acknowledge") == 0)
					printf("Confirmación de mensaje con identificador %d, realizada.\n", ack_message.identificador_mensaje);
			}
			else if(ack_message.identificador_mensaje < identificador_mensaje)
			{
				if(strcmp(ack_message.arguments, "Acknowledge") == 0) // SE TRATA DE LA CONFIRMACIÓN DE UN MENSAJE QUE APENAS VA LLEGANDO
					printf("Confirmación de mensaje con identificador %d, realizada.\n", ack_message.identificador_mensaje);
				else if(strcmp(ack_message.arguments, "Reenviar") == 0)
				{
					// UN RECEPTOR NO RECIBIÓ ALGÚN MENSAJE PREVIO, REENVIANDO 
					fprintf(stderr, "Un receptor no recibio el mensaje con identificador %d, reenviando...\n", ack_message.identificador_mensaje);

					// LLENANDO LA INFORMACIÓN NECESARIA PARA ENVIAR EL DATAGRAMA, LOS DATOS LOS OBTENEMOS DEL 
					// PAQUETE QUE RECIBIMOS PREVIAMENTE "paquete_recepcion"
					bzero((char *) &direccion_foranea, sizeof(direccion_foranea));
					direccion_foranea.sin_family = AF_INET;
					if(inet_pton(AF_INET, paquete_recepcion.obtieneDireccion(), &direccion_foranea.sin_addr) <= 0)
					{
						perror("Error: No se pudo hacer la conversión de la IP a formato de red");
						exit(1);
					}
					direccion_foranea.sin_port = htons(paquete_recepcion.obtienePuerto());

					// sleep(1);
					// ENVIANDO DATAGRAMA
					int cantidad_a_reenviar = 1;
					memcpy(&cantidad_a_reenviar, mensajes_almacenados[ack_message.identificador_mensaje].arguments, sizeof(int));
					printf("Cantidad a reenviar: %d\n", cantidad_a_reenviar);
					sendto(s, (char *) &cantidad_a_reenviar, sizeof(int), 0, (struct sockaddr *) &direccion_foranea, sizeof(direccion_foranea));
					printf("Mensaje reenviado.\n");	
				}		
			}
		}
	}
	
	identificador_mensaje++;	
	return everyReceiverGotMessage;
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