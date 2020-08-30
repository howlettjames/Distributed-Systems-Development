#include <iostream>
using namespace std;

class Coordenada
{
	private:
		double x;
		double y;
		double magnitud;
		
	public:
		// double magnitud;
		Coordenada(double = 0, double = 0);
		double obtenerX();
		double obtenerY();
		double obtenerMagnitud();
};

