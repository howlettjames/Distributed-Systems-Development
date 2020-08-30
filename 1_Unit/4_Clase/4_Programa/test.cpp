#include <iostream>
#include "Rectangulo.h"

using namespace std;

int main()
{
	// Rectangulo rectangulo1(2,3,5,1);
	// Rectangulo rectangulo1(Coordenada(2, 3), Coordenada(5, 1));
	Rectangulo rectangulo1(Coordenada(3.6055, 0.9827), Coordenada(5.0990, 0.1973));
	
	cout << "Calculando el área de un rectángulo dadas sus coordenadas en un plano cartesiano:\n";

	rectangulo1.imprimeEsq();

	cout << "El área del rectángulo es = " << rectangulo1.obtieneArea() << endl;

	return 0;
}