#include <iostream>
#include <cmath>
#include "Number.h"

using namespace std;

Number::Number(int n, bool p) : num(n), prime(p)
{
}

int Number::getNum()
{
	return num;
}

void Number::setNum(int n)
{
	num = n;
}

bool Number::isPrime()
{
	return prime;
}

void Number::setPrime(bool p)
{
	prime = p;
}
