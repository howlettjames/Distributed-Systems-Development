#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "Fecha.h"

using namespace std;

int masVieja(Fecha *fecha1, Fecha *fecha2);

int main()
{
	Fecha fecha1(2, 3, 1990), fecha2(3, 3, 1990);
	register int i = 0;

	srand(time(NULL));

	while(i < 2000000)
	{
		//cout << "Mas vieja: " << masVieja(fecha1, fecha2) << endl;
		masVieja(&fecha1, &fecha2);
		fecha1.inicializaFecha(rand() % 2021, (rand() % 12) + 1, (rand() % 31) + 1);
		fecha2.inicializaFecha(rand() % 2021, (rand() % 12) + 1, (rand() % 31) + 1);
		
		i++;
	}
}

int masVieja(Fecha *fecha1, Fecha *fecha2)
{
	if(fecha1->getAnio() == fecha2->getAnio())
		if(fecha1->getMes() == fecha2->getMes())
			if(fecha1->getDia() == fecha2->getDia())
				return 0;	
			else if(fecha1->getDia() > fecha2->getDia())
				return 1;
			else
				return -1;
		else if(fecha1->getMes() > fecha2->getMes())
			return 1;
		else
			return -1;
	else if(fecha1->getAnio() > fecha2->getAnio())
		return 1;
	else
		return -1;	
}
