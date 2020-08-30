#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstdlib>
#include <ctime> //time
#include <unistd.h>
#include <string.h> //memcpy
#include <thread>
#include <cmath>

using namespace std;

#define N 1757600
#define numeroElementos 100000000
char buffer[7030400];

void funcion1()
{
	int *arreglo, i;
	arreglo = new int[numeroElementos];
	
	for(i = 0; i < numeroElementos; i++)
		arreglo[i] = 0;

	for(i = 0; i < 140000000; i++)
		arreglo[rand() % numeroElementos] = rand();
}

void funcion2()
{
	unsigned char i;
	string cadenota;
	int destino;
	
	// GENERANDO N CADENAS DEL TIPO "ABC " RANDOM Y PEGANDOLAS TODAS EN VAR CADENOTA
	for(int n = 0; n < N; n++)
	{
		for(i = 0; i < 3; i++)
			cadenota.append(1, (rand() % 26) + 65);
		
		cadenota.append(" ");
	}

	// ABRIENDO ARCHIVO PARA GUARDAR CADENOTA
	if((destino = open("cadenota.txt", O_WRONLY|O_TRUNC|O_CREAT, 0666)) == -1)
	{
		perror("cadenota.txt");
		exit(-1);
	}
	
	// TRANSFORMANDO CADENOTA A UNA CADENA DEL TIPO C PARA PODER HACER LA ESCRITURA
	memcpy(buffer, cadenota.c_str(), cadenota.length());
	
	// ESCRIBIENDO CARACTER POR CARACTER EN EL ARCHIVO
	for(register int i = 0; i < cadenota.length(); i++)
		write(destino, buffer + i, 1);

	// FORZAMOS A QUE LA INFORMACIÓN CONTENIDA EN LOS BUFFERS DE LA LIBRERÍA Y DEL OS SEAN VACÍADOS EN DISCO
	fsync(destino);
	close(destino);
}

int main()
{
	srand((unsigned) time(0));
	
	thread th1(funcion1), th2(funcion2);
	
	th1.join();
	th2.join();
	
	exit(0);
}