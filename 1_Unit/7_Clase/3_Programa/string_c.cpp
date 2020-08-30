#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime> //time
#include <unistd.h>
#include <string.h> //memcpy

using namespace std;

#define N 300000

int main(int argc, char const *argv[])
{
	srand((unsigned) time(0));
	unsigned char i;
	char *cadena = (char *) malloc(5);
	char *cadenota = (char *) malloc(1);
	int posicion = 0, n, found;

	for(n = 0; n < N; n++)
	{
		for(i = 0; i < 3; i++)
			cadena[i] = (rand() % 26) + 65;
		
		cadena[3] = ' ';
		// cadena[4] = '\0';
		
		posicion += 4;
		cadenota = (char *) realloc(cadenota, posicion);	

		if(n == 0)
			memcpy(cadenota, cadena, 4);
		else
			memcpy(cadenota + posicion - 4, cadena, 4);
	}
	
	cadenota[posicion] = '\0';
	// cout << cadenota << endl;
	
	found = 0;
	for(n = 0; n < posicion; n += 4)
	{
		if(cadenota[n] == 'I')
			if(cadenota[n + 1] == 'P')
				if(cadenota[n + 2] == 'N')
					found++;
	}

	cout << "Found: " << found << endl;
	
	return 0;
}