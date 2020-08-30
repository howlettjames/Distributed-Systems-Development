#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h> 	// Para struct timeval
#include <vector>  		// Vector
#include <algorithm> 	// sort()
#include <unistd.h>		// sleep()
#include <sstream>		// stringstream

#include "Respuesta.h"
#include "Solicitud.h"
#include "registro.h"

using namespace std;

const int ALPHABET_SIZE = 10;				// Tamaño del alfabeto que estaremos manejando en el Trie, en este caso los números 0-9.

// #define SERVIDOR_IP_1 "127.0.0.1"
// #define SERVIDOR_IP_1 "192.168.100.99" 		// Cel
#define SERVIDOR_IP_1 "25.142.15.73"			// Alex
// #define SERVIDOR_IP_3 "25.121.204.24"			// Jaime

#define SERVIDOR_IP_2 "127.0.0.1"
// #define SERVIDOR_IP_2 "25.47.119.146" 		// Esteban
// #define SERVIDOR_IP_2 "25.121.204.24"			// Jaime

#define SERVIDOR_IP_3 "127.0.0.1"
// #define SERVIDOR_IP_3 "192.168.100.110" 		// Compu
// #define SERVIDOR_IP_3 "25.78.221.110"		// Douglas
// #define SERVIDOR_IP_3 "25.121.204.24"			// Jaime


// trie node 
struct TrieNode 
{ 
    struct TrieNode *children[ALPHABET_SIZE]; 
  
    // isEndOfWord is true if the node represents 
    // end of a word 
    bool isEndOfWord; 
}; 

// FUNCIONES DE NUESTRO ÁRBOL TRIE
struct TrieNode *getNode(void);
void insert(struct TrieNode *root, string key);
bool search(struct TrieNode *root, string key);

int main(int argc, char const *argv[])
{
	int registros_fd;								// File descriptor del archivo en donde escribiremos los registros
	struct timeval timestamp;						// Estructura timeval permite almacenar la hora exacta
	struct TrieNode *root = getNode();				// Nodo raíz de nuestro árbol Trie
	stringstream stream;							// Var para creación de nombre de archivo a escribir
	struct mensaje *men_respuesta;					// Var para recibir acuses de los demás servidores
	struct timeval timestamp_server1;				// Var para recibir timestamp de server 1
	struct timeval timestamp_server2;				// Var para recibir timestamp de server 2
	char timestamps[sizeof(struct timeval) * 2];	// Var para mandar dos timestamps en un solo mensaje					
	char acuse[] = "Timestamp(s) recibido(s)";		// Var para acuse hacia el otro servidor
	char mensaje_salida[] = "Saliendo del grupo";	// Var para avisar al otro servidor que este servidor se sale del grupo
	char mensaje_only_one[] = "Only you";			// Var para avisar al otro servidor que es el único que queda
	bool isServer1Alive = true, isServer2Alive = true, isServer3Alive = true;				
								
	// CHECANDO QUE LA INVOCACIÓN SEA CORRECTA
	if(argc != 3)
	{
		printf("Forma de uso: %s <puerto_a_escuchar> <numero_servidor>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	
	// CREANDO NOMBRE DEL ARCHIVO A ESCRIBIR DE ACUERDO A PUERTO A RECIBIR REGISTROS	
	int puerto_a_escuchar = atoi(argv[1]);
	stream << "servidor_registros" << puerto_a_escuchar << ".dat";
	if((registros_fd = open(stream.str().c_str(), O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1)
	{
		perror("servidor_registros.dat");
		exit(-1);
	}

	// ALMACENANDO EN VAR NÚMERO DE ESTE SERVIDOR
	int num_server = atoi(argv[2]);

	// CREAMOS UN OBJETO RESPUESTA PARA ATENDER AL CLIENTE
	Respuesta respuesta(atoi(argv[1]));
	// Respuesta respuesta1(8201);
	// Respuesta respuesta2(8202);
	
	// NECESARIO PARA PODER OCUPAR LOS PUERTOS SI SE CORRE LOCALMENTE
	Respuesta* respuesta1 = new Respuesta(1500);
	Respuesta* respuesta2 = new Respuesta(1501);
	if(num_server == 1)
	{
		delete respuesta1;	
		delete respuesta2;	
	}
	else if(num_server == 2)
	{
		delete respuesta1;
		delete respuesta2;	
		respuesta1 = new Respuesta(8201);
	}
	else if(num_server == 3)
	{
		delete respuesta1;
		delete respuesta2;	
		respuesta2 = new Respuesta(8202);
	}

	// OBJETOS PARA MANDAR SOLICITUES(TIMESTAMPS) A OTROS SERVIDORES
	Solicitud solicitud;
	Solicitud solicitud1;
							
	// VAR PARA ALMACENAR EL MENSAJES RECIBIDOS
	struct mensaje *mensaje_recv;
	struct mensaje *mensaje_recv1;
	struct mensaje *mensaje_recv2;
	
	// CREAMOS UNA VAR DE REGISTRO PARA ALMACENAR TEMPORALMENTE LOS REGISTROS RECIBIDOS
	struct registro_time registro_recibido;
	// EL SERVIDOR CORRE INDEFINIDAMENTE, ESCUCHANDO EN EL PUERTO 7200
	while(1)
	{
		// printf("Esperando solicitud...\n");
		
		// GUARDAMOS LA REFERENCIA AL MENSAJE RECIBIDO
		mensaje_recv = respuesta.getRequest();

		// IMPRIMIMOS LA INFORMACIÓN RECIBIDA 
		// cout << "SOLICITUD RECIBIDA" << endl;
		// cout << "Tipo de mensaje: " << mensaje_recv->messageType << endl;
		// cout << "ID de solicitud: " << mensaje_recv->requestId << endl;
		// cout << "ID de operación: " << mensaje_recv->operationId << endl;
		
		// EJECUTAMOS LA OPERACIÓN SOLICITADA
		if(mensaje_recv->operationId == guardar_registro)
		{
			// COPIAMOS EL(LOS) ARGUMENTOS(REGISTRO) EN LA VAR REGISTRO_RECIBIDO
			memcpy(&registro_recibido, mensaje_recv->arguments, sizeof(struct registro));

			// CREANDO UNA INSTANCIA DE OBJETO STRING A PARTIR DE LA CADENA ESTILO C CLÁSICA, EN ESTE CASO DEL CELULAR DE QUIEN SE HIZO EL REGISTRO
			string celularx(registro_recibido.celular);

			// HACEMOS LA BÚSQUEDA DE ESTE CELULAR SOBRE NUESTRO ÁRBOL TRIE
			if(search(root, celularx))
			{
				cout << "Ya existía este celular: " << celularx << endl;
				// sleep(3);
				// EN CASO DE EXISTIR PREVIAMENTE EL CELULAR EN NUESTRA BD, REGRESAMOS UN TIMSTAMP CON SEGUNDOS Y MICROSEGUNDOS IGUALES A 0
				registro_recibido.timestamp.tv_sec = 0;
				registro_recibido.timestamp.tv_usec = 0;
			}
			else
			{
				// INSERTAMOS EL CELULAR EN NUESTRO ÁRBOL TRIE
				insert(root, celularx);
				// OBTENIENDO LA HORA EXACTA ACTUAL, PARA GUARDAR EL REGISTRO JUNTO CON LA HORA EN QUE FUE ALMACENADO EN LA BD
				gettimeofday(&registro_recibido.timestamp, NULL);

				if(num_server == 1)
				{
					if(isServer2Alive)
					{
						// ENVIAMOS NUESTRO TIMESTAMP AL SERVIDOR ||2|| PARA QUE VERIFIQUE, ANTES DE RESPONDER, QUE NO SE REPITE EL MISMO TIMESTAMP
						men_respuesta = solicitud.doOperation(SERVIDOR_IP_2, 8201, timestamp_, (char *) &registro_recibido.timestamp, sizeof(struct timeval));				
						// printf("1: Server 2 dice: %s\n", men_respuesta->arguments);
						if(men_respuesta->arguments[0] == 'S')
						{
							printf("1: Cambiando a server 3\n");
							isServer2Alive = false;
							solicitud.requestId--;
						}
						else if(men_respuesta->arguments[0] == 'O')
						{
							isServer2Alive = false;
							isServer3Alive = false;
							printf("1: Servidor 2 y 3 fuera\n");	
						}
					}
					if(!isServer2Alive)
					{
						if(isServer3Alive)
						{
							// ENVIAMOS NUESTRO TIMESTAMP AL SERVIDOR ||3|| PARA QUE VERIFIQUE, ANTES DE RESPONDER, QUE NO SE REPITE EL MISMO TIMESTAMP
							memcpy(timestamps, &registro_recibido.timestamp, sizeof(struct timeval));
							memcpy(timestamps + sizeof(struct timeval), &registro_recibido.timestamp, sizeof(struct timeval));
							// EN EL CASO QUE EL SERVIDOR 3 TAMBIÉN MUERA, EL SERVIDOR 1 SE QUEDARÁ ESPERANDO HASTA QUE EL LÍMITE DE TIEMPO SE ACABE
							men_respuesta = solicitud.doOperation(SERVIDOR_IP_3, 8202, timestamp_, timestamps, sizeof(struct timeval) * 2);
							// printf("1: Server 3 dice: %s\n", men_respuesta->arguments);
							if(men_respuesta->arguments[0] == 'S')
							{
								isServer3Alive = false;
								printf("1: Servidor 2 y 3 fuera\n");
							}
						}
					}
				}
				else if(num_server == 2)
				{
					// RECIBIMOS EL TIMESTAMP DEL SERVIDOR 1
					mensaje_recv1 = respuesta1->getRequest();
					memcpy(&timestamp_server1, mensaje_recv1->arguments, sizeof(struct timeval));
					// printf("2: Recibido timestamp de server 1: %ld : %ld\n", timestamp_server1.tv_sec, timestamp_server1.tv_usec);

					// CHECANDO QUE EL SERVIDOR 1 SIGA ACTIVO
					// if(timestamp_server1.tv_sec == 0)
					// 	isServer1Alive = false;
							
					// ENVIANDO ACUSE A SERVIDOR 1
					respuesta1->sendReply(acuse, strlen(acuse));
					
					// CHECAMOS QUE NUESTRO TIMESTAMP(SERVER 2) NO SEA IGUAL AL DEL SERVER 1	
					if(timestamp_server1.tv_usec == registro_recibido.timestamp.tv_usec)
						registro_recibido.timestamp.tv_usec++;	

					if(isServer3Alive)
					{
						// ENVIAMOS NUESTRO TIMESTAMP Y EL DEL SERVER 1 AL SERVIDOR 3 PARA QUE VERIFIQUE, ANTES DE RESPONDER, QUE NO SE REPITE EL MISMO TIMESTAMP
						memcpy(timestamps, &timestamp_server1, sizeof(struct timeval));
						memcpy(timestamps + sizeof(struct timeval), &registro_recibido.timestamp, sizeof(struct timeval));
						men_respuesta = solicitud1.doOperation(SERVIDOR_IP_3, 8202, timestamp_, timestamps, sizeof(struct timeval) * 2);
						// printf("2: Server 3 dice: %s\n", men_respuesta->arguments);
						if(men_respuesta->arguments[0] == 'S')
						{
							isServer3Alive = false;
							printf("2: Cambiando solo a servidor 1\n");
						}
					}
				}
				else if(num_server == 3)
				{
					// RECIBIMOS EL TIMESTAMP DEL SERVIDOR 1 Y 2, O PUEDE QUE SOLO DEL SERVIDOR 1 O DEL 2, EN CUYO CASO RECIBIREMOS EL SEGUNDO 
					// TIMESTAMP IDÉNTICO AL PRIMERO
					mensaje_recv2 = respuesta2->getRequest();
					memcpy(&timestamp_server1, mensaje_recv2->arguments, sizeof(struct timeval));
					memcpy(&timestamp_server2, mensaje_recv2->arguments + sizeof(struct timeval), sizeof(struct timeval));
					// printf("3: Recibido timestamp de server 1: %ld : %ld\n", timestamp_server1.tv_sec, timestamp_server1.tv_usec);
					// printf("3: Recibido timestamp de server 2: %ld : %ld\n", timestamp_server2.tv_sec, timestamp_server2.tv_usec);

					// ENVIANDO ACUSE AL SERVIDOR 2 O 1, DEPENDE QUIEN HAYA MANDADO LOS TIMESTAMPS
					respuesta2->sendReply(acuse, strlen(acuse));
					
					// CHECANDO QUE NUESTRO TIMESTAMP(SERVER 3) NO SEA IGUAL AL DEL SERVIDOR 1 O 2
					if(timestamp_server1.tv_usec == registro_recibido.timestamp.tv_usec)
						registro_recibido.timestamp.tv_usec++;	
					if(timestamp_server2.tv_usec == registro_recibido.timestamp.tv_usec)
						registro_recibido.timestamp.tv_usec++;	
				}

				// ESCRIBIENDO EL REGISTRO EN EL ARCHIVO INDICADO POR LÍNEA DE COMANDOS
				write(registros_fd, &registro_recibido, sizeof(struct registro_time));
				// HACEMOS FSYNC() PARA QUE EL REGISTRO SEA ESCRITO INMEDIATAMENTE DE RAM A DISCO
				fsync(registros_fd);
				// ENVIAMOS LA RESPUESTA AL CLIENTE
				respuesta.sendReply((char *) &registro_recibido.timestamp, sizeof(struct timeval));

				// IMPRIMIMOS LA INFORMACIÓN DEL REMITENTE
				// printf("Cel: %s || CURP: %s || Partido: %s\n\n", registro_recibido.celular, registro_recibido.CURP, registro_recibido.partido);
				// printf("Segundos : Microsegundos -> %ld : %ld\n", registro_recibido.timestamp.tv_sec, registro_recibido.timestamp.tv_usec);
			}
		}
		else if(mensaje_recv->operationId == cerrando_comunicacion)
		{
			if(num_server == 2)
			{
				// AVISANDO A SERVIDOR 1 QUE SERVIDOR 2 ESTA FUERA
				mensaje_recv1 = respuesta1->getRequest();
				if(isServer3Alive)
					respuesta1->sendReply(mensaje_salida, strlen(mensaje_salida));	
				else
					respuesta1->sendReply(mensaje_only_one, strlen(mensaje_only_one));	
			}
			else if(num_server == 3)
			{
				// AVISANDO A SERVIDOR 2 QUE SERVIDOR 3 ESTA FUERA
				mensaje_recv2 = respuesta2->getRequest();
				respuesta2->sendReply(mensaje_salida, strlen(mensaje_salida));
			}

			printf("%d: terminando ejecucion...\n", num_server);
			// ENVIANDO LO QUE SEA AL CLIENTE, PARA QUE DEJE DE ESPERAR Y TERMINE SU HILO
			respuesta.sendReply((char *) &registro_recibido.timestamp, sizeof(struct timeval));
			// CERRANDO EL ARCHIVO Y SALIENDO
			close(registros_fd);
			exit(EXIT_SUCCESS);
		}
		else if(mensaje_recv->operationId == 0)
		{
			// SI EL ID DE OPERACIÓN ES CERO, EL SERVIDOR NO EFECTUARÁ NINGUNA OPERACIÓN, SE DESCARTA LA SOLICITUD REPETIDA
			continue;	
		}
	}

	return 0;
}
  
// Returns new trie node (initialized to NULLs) 
struct TrieNode *getNode(void) 
{ 
    struct TrieNode *pNode =  new TrieNode; 
  
    pNode->isEndOfWord = false; 
  
    for (int i = 0; i < ALPHABET_SIZE; i++) 
        pNode->children[i] = NULL; 
  
    return pNode; 
} 
  
// If not present, inserts key into trie 
// If the key is prefix of trie node, just 
// marks leaf node 
void insert(struct TrieNode *root, string key) 
{ 
    struct TrieNode *pCrawl = root; 
  
    for (int i = 0; i < key.length(); i++) 
    { 
        int index = key[i] - '0'; 
        if (!pCrawl->children[index]) 
            pCrawl->children[index] = getNode(); 
  
        pCrawl = pCrawl->children[index]; 
    } 
  
    // mark last node as leaf 
    pCrawl->isEndOfWord = true; 
} 
  
// Returns true if key presents in trie, else 
// false 
bool search(struct TrieNode *root, string key) 
{ 
    struct TrieNode *pCrawl = root; 
  
    for (int i = 0; i < key.length(); i++) 
    { 
        int index = key[i] - '0'; 
        if (!pCrawl->children[index]) 
            return false; 
  
        pCrawl = pCrawl->children[index]; 
    } 
  
    return (pCrawl != NULL && pCrawl->isEndOfWord); 
} 