#include <iostream>
#include <unistd.h>
#include "PoligonoIrreg.h"

using namespace std;

#define N 100000

int main()
{
	// EJERCICIO 2
	/*PoligonoIrreg pol1;
	
	pol1.anadirVertice(Coordenada(1, 1));
	pol1.anadirVertice(Coordenada(-1, 1));
	pol1.anadirVertice(Coordenada(1, -1));
	pol1.anadirVertice(Coordenada(-1, -1));
	
	pol1.imprimeVertices();
	*/

	int m = rand() % 100;
	cout << "M: " << m << endl;

	// EJERCICIO 3 B
	vector<PoligonoIrreg> poligonos;
	poligonos.reserve(N);

	for(int i = 0; i < N; i++)
	{
		for(int j = 0; j < m; j++)
			poligonos[i].anadirVertice(Coordenada(rand(), rand()));
	}
	cout << "Numero de vertices totales: " << poligonos[0].getNumeroVertices() << endl;
		
	return 0;
}