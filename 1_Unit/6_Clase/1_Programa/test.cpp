#include <iostream>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
#include "PoligonoIrreg.h"

using namespace std;

#define N 10

int main()
{
	srand((unsigned) time(0));
	int ran, ran1;
	double rando, rando1;
	
	// EJERCICIO 2
	PoligonoIrreg pol1;
	
	for(int i = 0; i < N; i++)
	{
		ran = (rand() % 200000) - 100000;
		rando = static_cast<double>(ran) / 1000;
		ran1 = (rand() % 200000) - 100000;
		rando1 = static_cast<double>(ran1) / 1000;
		
		pol1.anadirVertice(Coordenada(rando, rando1));	
	}
	
	pol1.imprimeVertices();
	pol1.ordenaA();
	printf("\n");
	pol1.imprimeVertices();
		

	return 0;
}