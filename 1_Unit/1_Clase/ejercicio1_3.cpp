#include <iostream>
using namespace std;

int main()
{
	double c = 0;

	cout << "!!!Hello!!!.\n" << "Enter the centigrade: ";
	cin >> c;

	// double f = 9.0/5.0 * c + 32.0;

	double f = static_cast<double>(9)/static_cast<double>(5) * c + 32.0;

	cout << "The area is: " << f << endl;

	return 0;
}