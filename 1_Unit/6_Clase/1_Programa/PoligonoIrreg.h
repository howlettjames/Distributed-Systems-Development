#include <iostream>
#include <vector>
#include "Coordenada.h"

using namespace std;

class PoligonoIrreg
{
	private:
		vector<Coordenada> vertices;
		static int nVertices;

	public:
		PoligonoIrreg();
		void anadirVertice(Coordenada nuevoVertice);
		void imprimeVertices();
		static int getNumeroVertices();
		void ordenaA();
};
