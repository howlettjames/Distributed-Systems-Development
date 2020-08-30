#include <iostream>
using namespace std;

class Coordenada
{
	private:
		double r;
		double t;
		double x;
		double y;

	public:
		Coordenada(double = 0, double = 0);
		double obtenerR();
		double obtenerT();
		double obtenerX();
		double obtenerY();
};

