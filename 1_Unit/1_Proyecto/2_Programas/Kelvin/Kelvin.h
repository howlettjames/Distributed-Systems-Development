#ifndef KELVIN_H_
#define KELVIN_H_

class Kelvin
{
	private:
		double kelvin;

 	public:
 		Kelvin(double kelvinn = 0.0);
 		void setKelvin(double kelvinn);
 		void setFahrenheit(double fahrenheit);
 		void setCelsius(double celsius);
 		void printKelvin();
 		void printFahrenheit();
 		void printCelsius();
};

#endif