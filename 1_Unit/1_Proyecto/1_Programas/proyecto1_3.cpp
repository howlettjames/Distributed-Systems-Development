#include <iostream>
using namespace std;

int main()
{
	
	int seconds, minutes, hours;

	cout << "Enter the seconds: ";
	cin >> seconds;

	minutes = seconds / 60;
	seconds = seconds % 60;
	hours = minutes / 60;
	minutes = minutes % 60;

	cout << "Hours: " << hours << ". Minutes: " << minutes << ". Seconds: " << seconds << endl;

	return 0;
}