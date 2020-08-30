#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <algorithm>
#include <ctime>
#include "SpaceObject.h"

using namespace std;

SpaceObject::SpaceObject(int xx, int yy, int dxx, int dyy, int nnSize, float anglee) : x(xx), y(yy), dx(dxx), dy(dyy), nSize(nnSize), angle(anglee)
{
	// CREAMOS EL MODELO PARA EL ASTEROIDE
	int verts = 20;								// Número de vértices
	float noise = 1.0f;							// Var para ajustar el "ruido" para que no se vea como un círculo perfecto
	for(int i = 0; i < verts; i++)				
	{
		// EL ERROR/RUIDO POR VERTICE VARIA DE MANERA ALEATORIA, DESDE UNA PEQUEÑA ALTERACIÓN
		// HASTA UNA MUY GRANDE, DE UNA PROBABILIDAD GRANDE A UNA PEQUEÑA RESPECTIVAMENTE
		if(rand() % 50 == 0)									
			noise = (float)rand() / (float)RAND_MAX * 0.3f + 0.3f;
		else if(rand() % 30 == 0)
			noise = (float)rand() / (float)RAND_MAX * 0.3f + 0.5f;
		else if(rand() % 20 == 0)
			noise = (float)rand() / (float)RAND_MAX * 0.3f + 0.7f;
		else
			noise = (float)rand() / (float)RAND_MAX * 0.3f + 0.9f;

		// EL RADIO DE CADA ASTEROIDE
		float asteroidsRadius = 10.0f;
		vecModelAsteroid.push_back(make_pair(round(asteroidsRadius * noise * sinf(((float)i / (float)verts) * 6.28318f)), 
											 round(asteroidsRadius * noise * cosf(((float)i / (float)verts) * 6.28318f))));
	}
	
}
