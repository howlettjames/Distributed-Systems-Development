#include <iostream>
using namespace std;

class Number
{
	private:
		int num;
		bool prime;

	public:
		Number(int num = 0, bool prime = true);
		int getNum();
		bool isPrime();
		void setNum(int n);
		void setPrime(bool p);
};

