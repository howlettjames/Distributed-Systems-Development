#define _USE_MATH_DEFINES
 
#include <cmath>
#include <iostream>
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
#include <mutex>
#include <thread>

using namespace std;

#define DIR_IP "127.0.0.1"
int puerto = 7200;
struct sockaddr_in msg_to_server_addr, client_addr;
int s;

// SERIE DE FOURIER DE UNA SEÑAL PERIÓDICA CUADRADA CON T = 2PI, 
// DEFINIDA COMO 0 DE [0, PI] Y 3 DE [PI, 2PI]
// ESTA SERIE NO TIENE COEFICIENTES an
double n = 1.0;                           
double a0 = 1.5;
double bn = 0.0;
double coordinates[1257 * 2];
int npoint = 0;
int confirmacion;
bool running = true;
bool first_time = true;

mutex m;

void funcion(void)
{
   while(running)
   {
      m.lock();
      // CALCULAMOS bn PORQUE ESTA SERIE SOLO TIENE ESOS COEFICIENTES
      bn = ( -3.0 / (n * M_PI) * (cos(n * M_PI) - 1) );
      
      cout << "n: " << n << " bn: " << bn << endl;

      if(bn != 0)
      {
         npoint = 0;
         for(double t = -2 * M_PI; t <= 2 * M_PI; t += 0.01)
         {
            // LA SUMA DE LOS TÉRMINOS POR CADA PUNTO ES ALMACENADA EN EL ARREGLO DE PUNTOS "SUM_POINTS"
            coordinates[npoint++] = t;
            coordinates[npoint++] += bn * sin(t * n);
         }

         if(first_time)
         {
            npoint /= 2;
            sendto(s, (char *) &npoint, sizeof(int), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));   
            first_time = false;

            // ENVIANDO LAS 1257(COORDENADAS) * 2 = 2514(npoint)
            sendto(s, (char *) coordinates, 2 * npoint * sizeof(double), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));   
         }
         else
         {
            // ENVIANDO LAS 1257(COORDENADAS) * 2 = 2514(npoint) 
            sendto(s, (char *) coordinates, npoint * sizeof(double), 0, (struct sockaddr *) &msg_to_server_addr, sizeof(msg_to_server_addr));
         }
         
         // RECIBIENDO CONFIRMACIÓN
         recvfrom(s, (char *) &confirmacion, sizeof(int), 0, NULL, NULL);

         if(confirmacion == 0)
            running = true;
         else
            running = false;

      }
      n++;   
      m.unlock();
   }
}
   
int main(void)
{
   // CREAMOS EL SOCKET   
   s = socket(AF_INET, SOCK_DGRAM, 0);

   // RELLENAMOS LA INFORMACIÓN DEL SERVIDOR
   bzero((char *)&msg_to_server_addr, sizeof(msg_to_server_addr));
   msg_to_server_addr.sin_family = AF_INET; 
   if(inet_pton(AF_INET, DIR_IP, &msg_to_server_addr.sin_addr) <= 0) //inet_pton - Convierte direcciones de texto IPv4 en forma binaria
   {
      perror("Ocurrio un error al momento de asignar la direccion IP");
      exit(1);
   }
   msg_to_server_addr.sin_port = htons(puerto);

   // RELLENAMOS LA INFORMACIÓN DEL CLIENTE
   bzero((char *)&client_addr, sizeof(client_addr));
   client_addr.sin_family = AF_INET;
   client_addr.sin_addr.s_addr = INADDR_ANY;

   // CUANDO SE COLOCA 0, EL SO SE ENCARGA DE ASIGNAR UNO
   client_addr.sin_port = htons(0);
   bind(s, (struct sockaddr *)&client_addr, sizeof(client_addr));
   
   // INICIALIZAMOS CADA PUNTO DE LA SERIE CON EL PRIMER TÉRMINO I.E. A0
   for(double t = -2 * M_PI; t <= 2 * M_PI; t += 0.01)
   {
      npoint++;
      coordinates[npoint++] = a0;
   }
   // LANZAMOS LOS HILOS
   thread hilo1(funcion), hilo2(funcion);
   
   hilo1.join();
   hilo2.join();
   
   close(s);
}

