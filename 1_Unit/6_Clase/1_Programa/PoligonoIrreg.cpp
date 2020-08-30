#include <iostream>
#include <algorithm>
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
		printf("Vertice numero: %d con coordenadas: (%7.3lf,%8.3lf) y magnitud: %10lf\n", 
				i + 1, vertices[i].obtenerX(), vertices[i].obtenerY(), vertices[i].obtenerMagnitud());
}

int PoligonoIrreg::nVertices = 0;

int PoligonoIrreg::getNumeroVertices()
{
	return nVertices;
}

void PoligonoIrreg::ordenaA()
{
	sort(vertices.begin(), vertices.end(), [](Coordenada &a, Coordenada &b)
	{
		return a.obtenerMagnitud() < b.obtenerMagnitud();
	});
}