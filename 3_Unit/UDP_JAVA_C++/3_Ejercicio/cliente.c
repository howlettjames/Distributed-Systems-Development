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

int main(void)
{
   struct sockaddr_in msg_to_server_addr, client_addr;
   int s;
   int ints[2];
   short shorts[2];
   long longs[2];
   float floats[2];
   double doubles[2];
   
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
   ints[0] = INT_MIN;
   ints[1] = INT_MAX; 
   sendto(s, (char *) ints, 2 * sizeof(int), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));
   
   // ENVIANDO SHORTS
   shorts[0] = SHRT_MIN;
   shorts[1] = SHRT_MAX; 
   sendto(s, (char *) shorts, 2 * sizeof(short), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));
   
   // ENVIANDO LONGS
   longs[0] = LONG_MIN;
   longs[1] = LONG_MAX; 
   sendto(s, (char *) longs, 2 * sizeof(long), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));
   
   // ENVIANDO FLOATS
   floats[0] = FLT_MIN;
   floats[1] = FLT_MAX; 
   sendto(s, (char *) floats, 2 * sizeof(float), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));
   
   // ENVIANDO DOUBLES
   doubles[0] = DBL_MIN;
   doubles[1] = DBL_MAX; 
   sendto(s, (char *) doubles, 2 * sizeof(double), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));
   
   close(s);
}

