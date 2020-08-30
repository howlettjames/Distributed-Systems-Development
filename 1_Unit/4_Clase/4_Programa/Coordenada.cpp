#include <iostream>
#include <cmath>
#include "Coordenada.h"

using namespace std;

Coordenada::Coordenada(double rr, double tt) : r(rr), t(tt)
{
	x = r * cos(t);
	y = r * sin(t);
}

double Coordenada::obtenerR()
{
	return r;
}

double Coordenada::obtenerT()
{
	return t;
}

double Coordenada::obtenerX()
{
	return x;
}

double Coordenada::obtenerY()
{
	return y;
}
