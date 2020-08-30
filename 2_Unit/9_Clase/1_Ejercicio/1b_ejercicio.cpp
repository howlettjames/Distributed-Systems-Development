#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string>
#include <cstdlib>
#include <ctime> //time
#include <unistd.h>
#include <string.h> //memcpy

using namespace std;

#define N 1757600

char buffer[7030400];

int main(int argc, char const *argv[])
{
	srand((unsigned) time(0));
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
		perror(argv[1]);
		exit(-1);
	}
	
	// TRANSFORMANDO CADENOTA A UNA CADENA DEL TIPO C PARA PODER HACER LA ESCRITURA
	memcpy(buffer, cadenota.c_str(), cadenota.length());
	
	// ESCRIBIENDO TOKEN(4 BYTES) POR TOKEN EN EL ARCHIVO
	for(register int i = 0; i < cadenota.length(); i += 4)
		write(destino, buffer + i, 4);

	// FORZAMOS A QUE LA INFORMACIÓN CONTENIDA EN LOS BUFFERS DE LA LIBRERÍA Y DEL OS SEAN VACÍADOS EN DISCO
	fsync(destino);
	close(destino);
	
	return 0;
}