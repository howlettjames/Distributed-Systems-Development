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

#define numeroElementos 100000000

using namespace std;

void funcion1()
{
	int *arreglo, i;
	arreglo = new int[numeroElementos];
	
	for(i = 0; i < numeroElementos; i++)
		arreglo[i] = 0;

	for(i = 0; i < 140000000; i++)
		arreglo[rand() % numeroElementos] = rand();
}

int main()
{
	srand((unsigned) time(0));
	
	thread th1(funcion1), th2(funcion1);
	
	th1.join();
	th2.join();
	
	exit(0);
}