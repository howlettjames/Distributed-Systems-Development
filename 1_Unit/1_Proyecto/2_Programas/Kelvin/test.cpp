#include <iostream>
#include "Kelvin.h"

using namespace std;

int main()
{
	Kelvin k, f, c;

	k.setKelvin(400.0);
	f.setFahrenheit(400.0);
	c.setCelsius(200.0);
	
	k.printKelvin();
	k.printFahrenheit();
	k.printCelsius();	
}