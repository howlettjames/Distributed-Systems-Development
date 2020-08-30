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

#define MAX 105000000
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
	double i = 0;
	double seno = 0, coseno = 0, tangente = 0, loga = 0, raiz = 0;

	while(i < MAX)
	{
		seno += sin(i);
		coseno += cos(i);
		tangente += tan(i);
		loga += log(i);
		raiz += sqrt(i);
		i++;
	}
}

int main()
{
	srand((unsigned) time(0));
	
	thread th1(funcion1), th2(funcion2);
	
	th1.join();
	th2.join();
	
	exit(0);
}