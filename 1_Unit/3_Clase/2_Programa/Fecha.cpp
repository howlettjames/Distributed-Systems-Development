#include <iostream>
#include <ctime>
#include "Fecha.h"

using namespace std;

time_t now = time(0);
tm *ltm = localtime(&now);

Fecha::Fecha(int dd, int mm, int aaaa) : dia(dd), mes(mm), anio(aaaa)
{
	if((mes < 1) || (mes > 12))
	{
		cout << "Valor ilegal para el mes!\n";
		exit(1);
	}
	else if((dia < 1) || (dia > 31))
	{
		cout << "Valor ilegal para el dia!\n";
		exit(1);	
	}
	else if((anio < 0) || (anio > (ltm->tm_year + 1900)))
	{
		cout << "Valor ilegal para el anio!\n";
		exit(1);		
	}
}

void Fecha::inicializaFecha(int dd, int mm, int aaaa)
{
	anio = aaaa;
	mes = mm;
	dia = dd;
	return;
}

void Fecha::muestraFecha()
{
	cout << "La fecha es(dia-mes-año): " << dia << "-" << mes << "-" << anio << endl;
	return;
}

int Fecha::getAnio()
{
	return anio;
}

int Fecha::getMes()
{
	return mes;
}

int Fecha::getDia()
{
	return dia;
}