#include <iostream>
using namespace std;

const double G = 9.8;

int main()
{
	double time, vf, h;

	cout << "Enter the time: ";	
	cin >> time;

	vf = G * time;
	h = (vf * vf) / (2 * G);

	cout << "The height is: " << h << "m" << endl;

	return 0;
}