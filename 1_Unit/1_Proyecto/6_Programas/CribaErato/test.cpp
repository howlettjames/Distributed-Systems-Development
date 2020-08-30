#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime> //time
#include <unistd.h>
#include <string.h> //memcpy
#include <vector>
#include "Number.h"

using namespace std;

int main()
{
	vector<Number> numbers;
	int n_limit, i;

	cout << "Enter N: ";
	cin >> n_limit;	

	for(i = 0; i < n_limit; i++)
	{
		if(i == 0)
		{
			Number n(i + 1, false);
			numbers.push_back(n);
		}
		else
		{
			Number n(i + 1, true);
			numbers.push_back(n);
		}
		
	}

	int number = 2;
	while((number * number) <= n_limit)
	{
		for(i = number; i < n_limit; i++)
		{
				if(numbers[i].isPrime())
					if( (numbers[i].getNum() % number) == 0)
						numbers[i].setPrime(false);	
		}

		for(i = number; i < n_limit; i++)
		{
			if(numbers[i].isPrime())
			{
				number = numbers[i].getNum();
				break;
			}
		}
	}

	for(i = 0; i < n_limit; i++)
	{
		if(numbers[i].isPrime())
			printf("%d ", numbers[i].getNum());
	}
	printf("\n");
	
	return 0;
}