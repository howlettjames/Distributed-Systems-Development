#include <iostream>
#include "Coordenada.h"

using namespace std;

class Rectangulo
{
	private:
		Coordenada closeOrigin;
		Coordenada farOrigin;
	
	public:
		Rectangulo();
		Rectangulo(double x1, double y1, double z1, double x2, double y2, double z2);
		Rectangulo(Coordenada closeOrigin, Coordenada farOrigin);
		void obtenerVertices();
		Coordenada obtieneCloseOrigin();
		Coordenada obtieneFarOrigin();
		double obtieneArea();
		double obtieneVol();
};
