#include <iostream>
#include "Rectangulo.h"

using namespace std;

double Rectangulo::obtieneArea()
{
	double alto = superiorIzq.obtenerY() - inferiorDer.obtenerY();
	double ancho = inferiorDer.obtenerX() - superiorIzq.obtenerX();

	return alto * ancho;
} 

Rectangulo::Rectangulo() : superiorIzq(0,0), inferiorDer(0,0)
{
}

Rectangulo::Rectangulo(double xSupIzq, double ySupIzq, double xInfDer, double yInfDer):superiorIzq(xSupIzq, ySupIzq), inferiorDer(xInfDer, yInfDer)
{
}

Rectangulo::Rectangulo(Coordenada superiorIzq, Coordenada inferiorDer):superiorIzq(superiorIzq), inferiorDer(inferiorDer)
{
}

void Rectangulo::imprimeEsq()
{
	cout << "Para la esquina superior izquierda.\n";
	cout << "x = " << superiorIzq.obtenerX() << " y = " << superiorIzq.obtenerY() << endl;
	cout << "Para la esquina inferior derecha.\n";
	cout << "x = " << inferiorDer.obtenerX() << " y = " << inferiorDer.obtenerY() << endl;
}

Coordenada Rectangulo::obtieneSupIzq()
{
	return superiorIzq;
}

Coordenada Rectangulo::obtieneInfDer()
{
	return inferiorDer;
}