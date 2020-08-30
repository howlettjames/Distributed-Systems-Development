#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h> //memcpy
#include <arpa/inet.h> //inet_pton
#include "PaqueteDatagrama.h"

using namespace std;

class SocketMulticast
{
	public:
		SocketMulticast(int);
		~SocketMulticast();
		int recibe(PaqueteDatagrama &p);
		int recibeTimeout(PaqueteDatagrama &p, time_t segundos, suseconds_t microsegundos);
		int envia(PaqueteDatagrama &p, unsigned char ttl);
		void unirseGrupo(char *);
		void salirseGrupo(char *);
	private:
		int s;
		struct sockaddr_in direccion_local, direccion_foranea;
		struct timeval timeout;
};