#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <strings.h>
#include <limits.h>
#include <float.h>

#define DIR_IP "127.0.0.1"
int puerto = 7200;

// IMPORTA MUCHO EL ORDEN EN QUE DECLAREMOS LOS MIEMBROS DE LA ESTRUCTURA, 
// PARA OPTIMIZAR EL TAMAÑO DE TODA LA ESTRUCTURA DECLARAMOS EN ORDEN DE TAMAÑO EN BYTES,
// DE LO CONTRARIO SI DECLARAMOS UN ENTERO Y LUEGO UN LARGO, POR EJEMPLO, EL COMPILADOR
// DEJARÁ 4 BYTES DE ESPACIO ENTRE EL ENTERO Y EL LARGO, BYTES QUE SE DESPERDICIAN.
struct mensaje
{
   int entero;
   float flotante;
   double doble;
   long largo;
};

int main(void)
{
   struct sockaddr_in msg_to_server_addr, client_addr;
   int s;
   struct mensaje message;

   s = socket(AF_INET, SOCK_DGRAM, 0);

   /* rellena la dirección del servidor */
   bzero((char *)&msg_to_server_addr, sizeof(msg_to_server_addr));
   msg_to_server_addr.sin_family = AF_INET; 
   if(inet_pton(AF_INET, DIR_IP, &msg_to_server_addr.sin_addr) <= 0) //inet_pton - Convierte direcciones de texto IPv4 en forma binaria
   {
      perror("Ocurrio un error al momento de asignar la direccion IP");
      exit(1);
   }
   msg_to_server_addr.sin_port = htons(puerto);

   /* rellena la direcciòn del cliente*/
   bzero((char *)&client_addr, sizeof(client_addr));
   client_addr.sin_family = AF_INET;
   client_addr.sin_addr.s_addr = INADDR_ANY;

   /*cuando se utiliza por numero de puerto el 0, el sistema se encarga de asignarle uno */
   client_addr.sin_port = htons(0);
   bind(s, (struct sockaddr *)&client_addr, sizeof(client_addr));

   // ENVIANDO INTS
   message.entero = INT_MAX;
   message.flotante = FLT_MAX;
   message.largo = LONG_MAX;
   message.doble = DBL_MAX;

   printf("Tamanio de estructura mensaje: %ld\n", sizeof(struct mensaje));

   sendto(s, (char *) &message, sizeof(struct mensaje), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));

   close(s);
}

