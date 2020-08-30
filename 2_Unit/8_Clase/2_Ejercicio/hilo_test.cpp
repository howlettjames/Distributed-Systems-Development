#include <iostream>
#include <unistd.h>
#include <thread>

using namespace std;

int variable = 0;

void funcion1()
{
	variable++;
	sleep(1);
}

void funcion2()
{
	variable--;
	sleep(1);
}

int main()
{
	thread th1(funcion1), th2(funcion2);

	cout << "Proceso principal espera que los hilos terminen\n";
	th1.join();
	th2.join();
	cout << "El hilo principal termina\n";
	cout << "Variable: " << variable << endl;

	exit(0);
}