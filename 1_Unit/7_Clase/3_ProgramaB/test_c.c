#include <stdio.h>
#include <math.h>

#define MAX 50000000

int main(int argc, char const *argv[])
{
	double i = 0;
	double seno = 0, coseno = 0, tangente = 0, loga = 0, raiz = 0;

	while(i < MAX)
	{
		seno += sin(i);
		coseno += cos(i);
		tangente += tan(i);
		loga += log(i);
		raiz += sqrt(i);
		i++;
	}

	return 0;
}