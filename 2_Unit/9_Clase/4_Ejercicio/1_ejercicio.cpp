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

#define N 1757600

char buffer[7030400];

using namespace std;

void funcion1(const char *filename)
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
	if((destino = open(filename, O_WRONLY|O_TRUNC|O_CREAT, 0666)) == -1)
	{
		perror(filename);
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
	
	thread th1(funcion1, "cadenota1.txt"), th2(funcion1, "cadenota2.txt");
	
	th1.join();
	th2.join();
	
	exit(0);
}