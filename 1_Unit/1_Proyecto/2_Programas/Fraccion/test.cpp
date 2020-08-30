#include <iostream>
#include "Fraccion.h"

using namespace std;

int main()
{
	Fraccion f;

	f.setNum(45);
	f.setDen(75);
	cout << "Fraction: " << f.getDouble() << endl;
	f.minimized();
}