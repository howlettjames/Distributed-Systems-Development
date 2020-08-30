#include <iostream>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h> 	// Para struct timeval
#include <vector>  		// Vector
#include <algorithm> 	// sort()
#include <unistd.h>		// sleep()

#include "Respuesta.h"
#include "registro.h"

using namespace std;

const int ALPHABET_SIZE = 10;				// Tamaño del alfabeto que estaremos manejando en el Trie, en este caso los números 0-9.

#define PUERTO_A_ESCUCHAR 7200

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

// bool compare_function(string a, string b) { return a < b; }

int main(int argc, char const *argv[])
{
	int registros_fd;								// File descriptor del archivo en donde escribiremos los registros
	struct timeval timestamp;						// Estructura timeval permite almacenar la hora exacta
	struct TrieNode *root = getNode();				// Nodo raíz de nuestro árbol Trie
			
	// CHECANDO QUE LA INVOCACIÓN SEA CORRECTA
	if(argc != 2)
	{
		printf("Forma de uso: %s <nombre_archivo_guardar_registros>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	// ABRIENDO ARCHVIO PARA ESCRITURA DE REGISTROS
	if((registros_fd = open(argv[1], O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1)
	{
		perror(argv[1]);
		exit(-1);
	}

	// CREAMOS UN OBJETO RESPUESTA PARA ATENDER AL CLIENTE
	Respuesta respuesta(PUERTO_A_ESCUCHAR);

	// VAR PARA ALMACENAR EL MENSAJE RECIBIDO
	struct mensaje *mensaje_recv;
	
	// CREAMOS UNA VAR DE REGISTRO PARA ALMACENAR TEMPORALMENTE LOS REGISTROS RECIBIDOS
	struct registro_time registro_recibido;
	// EL SERVIDOR CORRE INDEFINIDAMENTE, ESCUCHANDO EN EL PUERTO 7200
	// printf("Tam: %ld\n", sizeof(struct TrieNode));
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
		
		// COPIAMOS EL(LOS) ARGUMENTOS(REGISTRO) EN LA VAR REGISTRO_RECIBIDO
		memcpy(&registro_recibido, mensaje_recv->arguments, sizeof(struct registro));

		// EJECUTAMOS LA OPERACIÓN SOLICITADA
		if(mensaje_recv->operationId == guardar_registro)
		{
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
				// cout << "Este celular es nuevo, guardando el registro en la BD..." << endl;
				// INSERTAMOS EL CELULAR EN NUESTRO ÁRBOL TRIE
				insert(root, celularx);

				// OBTENIENDO LA HORA EXACTA ACTUAL, PARA GUARDAR EL REGISTRO JUNTO CON LA HORA EN QUE FUE ALMACENADO EN LA BD
				gettimeofday(&registro_recibido.timestamp, NULL);
				// ESCRIBIENDO EL REGISTRO EN EL ARCHIVO INDICADO POR LÍNEA DE COMANDOS
				write(registros_fd, &registro_recibido, sizeof(struct registro_time));
				// HACEMOS FSYNC() PARA QUE EL REGISTRO SEA ESCRITO INMEDIATAMENTE A DISCO
				fsync(registros_fd);

				// IMPRIMIMOS LA INFORMACIÓN DEL REMITENTE
				// printf("REGISTRO\n");
				// printf("Cel: %s\n", registro_recibido.celular);
				// printf("CURP: %s\n", registro_recibido.CURP);
				// printf("Partido: %s\n", registro_recibido.partido);
				// printf("Segundos:Microsegundos -> %ld : %ld\n", registro_recibido.timestamp.tv_sec, registro_recibido.timestamp.tv_usec);
				// printf("Registro almacenado en el archivo: %s\n", argv[1]);
			}

			// ENVIAMOS LA RESPUESTA
			respuesta.sendReply((char *) &registro_recibido.timestamp, sizeof(struct timeval));
			// printf("Confirmacion enviada.\n\n");

			// IMPRIMIENDO LOS CELULARES
			// for(auto i = celulares.begin(); i != celulares.end(); ++i)
			// {
			// 	string stringx = *i;
			// 	cout << stringx << endl;
			// }
			// printf("\n");
		}
		else if(mensaje_recv->operationId == 0)
		{
			// SI EL ID DE OPERACIÓN ES CERO, EL SERVIDOR NO EFECTUARÁ NINGUNA OPERACIÓN, SE DESCARTA LA SOLICITUD REPETIDA
			continue;	
		}
	}

	// CERRANDO EL ARCHIVO
	close(registros_fd);

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