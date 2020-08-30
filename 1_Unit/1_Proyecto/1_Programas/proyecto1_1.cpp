#include <iostream>
using namespace std;

int main()
{
	int num, i;
	double s, root;

	cout.setf(ios::fixed);
	cout.setf(ios::showpoint);
	cout.precision(4);

	cout << "Enter the integer: ";
	cin >> num;

	s = static_cast<double>(num);
	root = s / 2.0;

	i = 0;
	while(i < 10)
	{
		root = (root + (s / root)) / 2.0;
		i++;
	}

	cout << "The root is: " << root << endl;

	return 0;
}