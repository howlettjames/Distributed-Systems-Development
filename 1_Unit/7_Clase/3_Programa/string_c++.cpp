#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime> //time
#include <unistd.h>
#include <string.h> //memcpy

using namespace std;

#define N 300000

int main(int argc, char const *argv[])
{
	srand((unsigned) time(0));
	unsigned char i;
	string cadenota;
	int found;
	
	for(int n = 0; n < N; n++)
	{
		for(i = 0; i < 3; i++)
			cadenota.append(1, (rand() % 26) + 65);
		
		cadenota.append(" ");
	}

	found = 0;
	size_t pos = cadenota.find("IPN", 0);
	while(pos != string::npos)
	{
		found++;
		pos = cadenota.find("IPN", pos + 1);
	}

	cout << "Found: " << found << endl;
	// cout << cadenota << endl;

	return 0;
}