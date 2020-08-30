#include <iostream>
using namespace std;

const double PI = 3.1415926;

int main()
{
	double radius;

	cout << "!!!Hello!!!.\n" << "Enter the radius: ";
	cin >> radius;

	cout << "The area is: " << radius * radius * PI << endl;

	return 0;
}