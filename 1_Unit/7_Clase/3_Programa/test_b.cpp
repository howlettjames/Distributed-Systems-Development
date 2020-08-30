#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime> //time
#include <unistd.h>
#include <string.h> //memcpy

using namespace std;

#define N 3

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
		
		cadena[i++] = ' ';
		// cadena[i] = '\0';
		
		if(!n)
		{
			posicion = 5;
			cadenota = (char *) realloc(cadenota, posicion);
			// strcpy(cadenota, cadena);
			memcpy(cadenota, cadena, 5);
			continue;
		}
		
		posicion += 4;
		cadenota = (char *) realloc(cadenota, posicion);	
		// strcat(cadenota, cadena);
		memcpy(cadenota, cadena, 5);
	}

	found = 0;
	for(n = 0; n < posicion; n += 4)
	{
		if(cadenota[n] == 'I')
			if(cadenota[n + 1] == 'P')
				if(cadenota[n + 2] == 'N')
					found++;
	}

	cout << "Found: " << found << endl;
	cout << cadenota << endl;

	return 0;
}