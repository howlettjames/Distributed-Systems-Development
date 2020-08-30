#include <iostream>
#include "Fraccion.h"

using namespace std;

Fraccion::Fraccion(int numm, int denn)
{
	num = numm;
	den = denn;
}

void Fraccion::setNum(int numm)
{
	num = numm;
	return;
}

void Fraccion::setDen(int denn)
{
	den = denn;
	return;
}

double Fraccion::getDouble()
{
	return static_cast<double>(num) / static_cast<double>(den);
}

void Fraccion::minimized()
{
	int quotient, remainder, divisor, dividend, mcf;

	divisor = den;
	dividend = num;
	quotient = 0;
	remainder = 0;
	do
	{
		quotient = dividend / divisor;
		remainder = dividend % divisor;
		dividend = divisor;
		divisor = remainder;
	}
	while(remainder != 0);

	mcf = dividend;
	printf("%d/%d = %d/%d\n", num, den, num / mcf, den / mcf);
}