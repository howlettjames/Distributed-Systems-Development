#include <iostream>
#include "Kelvin.h"

using namespace std;

Kelvin::Kelvin(double kelvinn)
{
	kelvin = kelvinn;
}

void Kelvin::setKelvin(double kelvinn)
{
	kelvin = kelvinn;
	return;
}

void Kelvin::setFahrenheit(double fahrenheit)
{
	kelvin = (fahrenheit - 32.0) * 5.0/9.0 + 273.15;
	return;
}

void Kelvin::setCelsius(double celsius)
{
	kelvin = celsius + 273.15;
	return;
}

void Kelvin::printKelvin()
{
	cout << "Kelvin: " << kelvin << endl;
	return;
}

void Kelvin::printFahrenheit()
{
	cout << "Fahrenheit: " << (kelvin - 273.15) * 9.0/5.0 + 32.0 << endl;
	return;
}

void Kelvin::printCelsius()
{
	cout << "Celsius: " << (kelvin - 273.15) << endl;
	return;
}