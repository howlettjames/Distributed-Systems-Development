#include <iostream>
#include "PoligonoIrreg.h"

using namespace std;

PoligonoIrreg::PoligonoIrreg()
{
}

void PoligonoIrreg::anadirVertice(Coordenada nuevoVertice)
{
	vertices.push_back(nuevoVertice);
	nVertices++;
}

void PoligonoIrreg::imprimeVertices()
{
	for(int i = 0; i < vertices.size(); i++)
		cout << "Vertice numero " << i + 1 << " con coordenadas " << vertices[i].obtenerX() << "," << vertices[i].obtenerY() << endl; 
}

int PoligonoIrreg::nVertices = 0;

int PoligonoIrreg::getNumeroVertices()
{
	return nVertices;
}