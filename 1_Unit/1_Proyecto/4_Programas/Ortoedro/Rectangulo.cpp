#include <iostream>
#include "Rectangulo.h"

using namespace std;

Rectangulo::Rectangulo() : closeOrigin(0, 0, 0), farOrigin(0, 0, 0)
{
}

Rectangulo::Rectangulo(double x1, double y1, double z1, double x2, double y2, double z2) : closeOrigin(x1, y1, z1), farOrigin(x2, y2, z2)
{
}

Rectangulo::Rectangulo(Coordenada closeOriginn, Coordenada farOriginn) : closeOrigin(closeOriginn), farOrigin(farOriginn)
{
}

double Rectangulo::obtieneArea()
{
	double alto = farOrigin.obtenerZ() - closeOrigin.obtenerZ();
	double ancho = farOrigin.obtenerY() - closeOrigin.obtenerY();
	double prof = farOrigin.obtenerX() - closeOrigin.obtenerX();

	double areaBase = ancho * prof;
	double perimetroBase = 2 * (ancho + prof);
	double areaLateral = perimetroBase * alto;
	double areaTotal = 2 * areaBase + areaLateral;

	return areaTotal;
} 

double Rectangulo::obtieneVol()
{
	double alto = farOrigin.obtenerZ() - closeOrigin.obtenerZ();
	double ancho = farOrigin.obtenerY() - closeOrigin.obtenerY();
	double prof = farOrigin.obtenerX() - closeOrigin.obtenerX();

	return alto * ancho * prof;
}

void Rectangulo::obtenerVertices()
{
	printf("Vertice 1: %lf %lf %lf\n", closeOrigin.obtenerX(), closeOrigin.obtenerY(),  closeOrigin.obtenerZ());
	printf("Vertice 2: %lf %lf %lf\n", farOrigin.obtenerX(), farOrigin.obtenerY(),  farOrigin.obtenerZ());
	printf("Vertice 3: %lf %lf %lf\n", closeOrigin.obtenerX(), closeOrigin.obtenerY(),  farOrigin.obtenerZ()); 
	printf("Vertice 4: %lf %lf %lf\n", farOrigin.obtenerX(), farOrigin.obtenerY(),  closeOrigin.obtenerZ());   
	printf("Vertice 5: %lf %lf %lf\n", farOrigin.obtenerX(), closeOrigin.obtenerY(),  closeOrigin.obtenerZ());
	printf("Vertice 6: %lf %lf %lf\n", farOrigin.obtenerX(), closeOrigin.obtenerY(),  farOrigin.obtenerZ());
	printf("Vertice 7: %lf %lf %lf\n", closeOrigin.obtenerX(), farOrigin.obtenerY(),  farOrigin.obtenerZ());
	printf("Vertice 8: %lf %lf %lf\n", closeOrigin.obtenerX(), farOrigin.obtenerY(),  closeOrigin.obtenerZ());
}

Coordenada Rectangulo::obtieneCloseOrigin()
{
	return closeOrigin;
}

Coordenada Rectangulo::obtieneFarOrigin()
{
	return farOrigin;
}