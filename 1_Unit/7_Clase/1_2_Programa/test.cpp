#include <iostream>
#include <string>

using namespace std;


int main(int argc, char const *argv[])
{
	int n;
	string line;

	// EJEMPLO 1
	// cin >> n;
	// getline(cin, line);

	// EJEMPLO 2
	string cadena("ESCOM");
	cout << cadena[6] << endl;
	cout << cadena.at(6) << endl;

	return 0;
}