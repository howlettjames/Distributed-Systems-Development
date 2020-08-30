#include <iostream>
#include "Coordenada.h"

using namespace std;

class Rectangulo
{
	private:
		Coordenada superiorIzq;
		Coordenada inferiorDer;
	
	public:
		Rectangulo();
		Rectangulo(double xSupIzq, double ySupIzq, double xInfDer, double yInfDer);
		Rectangulo(Coordenada superiorIzq, Coordenada inferiorDer);
		void imprimeEsq();
		Coordenada obtieneSupIzq();
		Coordenada obtieneInfDer();
		double obtieneArea();
};
