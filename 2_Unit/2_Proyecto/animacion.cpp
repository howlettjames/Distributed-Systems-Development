#include <unistd.h>
#include <stdlib.h>
#include <cstdlib>
#include <math.h>
#include <algorithm>
#include <ctime>
#include "SpaceObject.h"
#include "gfx.h"

using namespace std;

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

void WrapCoordinates(int ix, int iy, int &ox, int &oy);
void DrawWireFrameModel(const vector<pair<int, int>> &vecModelCoordinates, int x, int y, float r, int s);

int main(int argc, char *argv[])
{
	unsigned int fElapsedTime = 1;
	char c;
	bool firstTime = true;
	int rando;
	vector<SpaceObject> vecAsteroids;

	// INICIALIZANDO SEMILLA RANDOM
	srand(time(NULL));
	
	//CHECANDO SI PASARON EL ARGUMENTO DE NÚMERO DE ASTEROIDES
	if(argc != 2)
	{
		printf("Program usage: %s <NumberOfAsteroids>\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	// ACEPTANDO EL NÚMERO DE ASTEROIDES DADOS EN LÍNEA DE COMANDOS
	int nAsteroides =  stoi(argv[1]);

	// CREAMOS EL ESPACIO Y ASIGNAMOS COLOR VERDE A LOS OBJETOS
	gfx_open(SCREEN_WIDTH, SCREEN_HEIGHT, "ASTEROIDES");
	gfx_color(0, 200, 100);

	// AGREGAMOS ASTEROIDES {x, y, dx, dy, nSize, angle}
	int size = 0;
	int velocity = 0;
	int x, y, dx, dy;
	for(int i = 0; i < nAsteroides; i++)
	{
		// HAY 5 TAMAÑOS DIFERENTES
		size = (rand() % 5) + 1;
		// LA VELOCIDAD ES INVERSAMENTE PROPORCIONAL AL TAMAÑO
		velocity = (int) round(4.0f / (float) size);

		// HACEMOS ALEATORIA LA APARICIÓN DE LOS ASTEROIDES A LOS LADOS
		rando = rand() % 4;
		if(rando == 0) // LADO IZQUIERDO
		{
			x = 0;
			y = rand() % SCREEN_HEIGHT;
			dx = rand() % 3;
			dy = rand() % 3;
		}	
		else if(rando == 1) // LADO SUPERIOR
		{
			x = rand() % SCREEN_WIDTH;
			y = 0;
			dx = rand() % 3;
			dy = rand() % 3;
		}
		else if(rando == 2) // LADO DERECHO
		{
			x = SCREEN_WIDTH;
			y = rand() % SCREEN_HEIGHT;
			dx = -(rand() % 3);
			dy = -(rand() % 3);
		}
		else // LADO INFERIOR
		{
			x = rand() % SCREEN_WIDTH;
			y = SCREEN_HEIGHT;
			dx = -(rand() % 3);
			dy = -(rand() % 3);
		}

		// CALCULAMOS ALEATORIAMENTE EL VECTOR DE VELOCIDAD DE LOS ASTEROIDES Y DE ACUERDO A SU TAMAÑO
		vecAsteroids.push_back({x, y, dx * velocity, dy * velocity, size, 0.0f});
	}
	
	// CORREMOS LA SIMULACIÓN POR TIEMPO INDEFINIDO
	while(true)
	{
		// LIMPIANDO EL ESPACIO
		gfx_clear();

		// RUTINA QUE DIBUJA ASTEROIDES
		int i = 0;
		for(auto &a: vecAsteroids)
		{	
			// ACTUALIZANDO POSICIÓN RESPECTO A VECTOR DE VELOCIDAD
			a.x += a.dx * fElapsedTime;
			a.y += a.dy * fElapsedTime;	

			// AJUSTANDO ROTACIÓN DE ACUERDO A TAMAÑO, INVERSAMENTE PROPORCIONAL
			if(a.nSize == 1)
				a.angle += 0.1f * fElapsedTime;
			else if(a.nSize == 2)
				a.angle += 0.09f * fElapsedTime;
			else if(a.nSize == 3)
				a.angle += 0.07f * fElapsedTime;
			else if(a.nSize == 4)
				a.angle += 0.05f * fElapsedTime;
			else if(a.nSize == 5)
				a.angle += 0.02f * fElapsedTime;
			
			// LAS COORDENADAS DEL ASTEROIDE SE MANTIENEN EN EL ESPACIO
			WrapCoordinates(a.x, a.y, a.x, a.y);
			// RUTINA PARA DIBUJAR EL ASTEROIDE CON LA ROTACIÓN, TAMAÑO Y COORDENADAS DESEADAS
			DrawWireFrameModel(a.vecModelAsteroid, a.x, a.y, a.angle, a.nSize);		
		}	

		// SI SE QUIERE DETENER LA EJECUCIÓN UNA SOLA VEZ
		/*
		while(firstTime) 
		{
			c = gfx_wait();
			if(c == 's')
			{
				firstTime = false;
				break;
			}
		}
		*/

		// SI SE DESEA VER LA EJECUCIÓN PASO A PASO, SE INGRESA LA LETRA "s" PARA CADA PASO, "q" PARA FINALIZAR
		/*while(1) 
		{
			c = gfx_wait();
			if(c == 's')
				break;
			else if(c == 'q')
				exit(EXIT_SUCCESS);
		}*/

		// FORZANDO A QUE SE DIBUJEN LOS OBJETOS
		gfx_flush();
		// FRAME DE 100 MS
		usleep(fElapsedTime * 100000); //
	}
	
	return 0;
}

void DrawWireFrameModel(const vector<pair<int, int>> &vecModelCoordinates, int x, int y, float r = 0, int s = 1)
{
	// pair.first = x coordinate
	// pair.second = y coordinate

	// Create translated model vector of coordinate pairs
	vector<pair<int, int>> vecTransformedCoordinates;
	int verts = vecModelCoordinates.size();
	vecTransformedCoordinates.resize(verts);

	for(int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].first = vecModelCoordinates[i].first;
		vecTransformedCoordinates[i].second = vecModelCoordinates[i].second;
	}

	// Rotate
	for(int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].first = round(vecModelCoordinates[i].first * cosf(r) - vecModelCoordinates[i].second * sinf(r));
		vecTransformedCoordinates[i].second = round(vecModelCoordinates[i].first * sinf(r) + vecModelCoordinates[i].second * cosf(r));
	}
	
	// Scale
	for (int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first * s;
		vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second * s;
	}

	// Translate
	for (int i = 0; i < verts; i++)
	{
		vecTransformedCoordinates[i].first = vecTransformedCoordinates[i].first + x;
		vecTransformedCoordinates[i].second = vecTransformedCoordinates[i].second + y;
	}

	// Draw Closed Polygon
	for (int i = 0; i < verts + 1; i++)
	{
		int j = (i + 1);
		
		// WrapCoordinates(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second);
		// WrapCoordinates(vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second, vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second);
		
		gfx_line(vecTransformedCoordinates[i % verts].first, vecTransformedCoordinates[i % verts].second, 
			vecTransformedCoordinates[j % verts].first, vecTransformedCoordinates[j % verts].second);
	}
}

void WrapCoordinates(int ix, int iy, int &ox, int &oy)
{
	ox = ix;
	oy = iy;

	if(ix < 0)
		ox = ix + SCREEN_WIDTH;
	if(ix >= SCREEN_WIDTH)
		ox = ix - SCREEN_WIDTH;
	
	if(iy < 0)
		oy = iy + SCREEN_HEIGHT;
	if(iy >= SCREEN_HEIGHT)
		oy = iy - SCREEN_HEIGHT;

}