#include <iostream>
#include "Rectangulo.h"

using namespace std;

int main()
{
	Rectangulo rectangulo1(Coordenada(1, 2, 3), Coordenada(3, 11, 5));
	
	cout << "Calculando el área de un ortoedro dadas sus coordenadas en el espacio:\n";

	rectangulo1.obtenerVertices();
	cout << "\nEl área del ortoedro es = " << rectangulo1.obtieneArea() << "cm2" << endl;
	cout << "El volumen del ortoedro es = " << rectangulo1.obtieneVol() << "cm3" << endl;

	return 0;
}