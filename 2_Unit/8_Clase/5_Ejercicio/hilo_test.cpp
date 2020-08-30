#include <iostream>
#include <unistd.h>
#include <thread>
#include <atomic>
#include <mutex>

using namespace std;

int variable = 0;

mutex m;
	
void funcion1(int n)
{
	m.lock();
	for(int i = 0; i < n; i++)
		variable++;
	m.unlock();

	sleep(1);
}

void funcion2(int n)
{
	m.lock();
	for(int i = 0; i < n; i++)
		variable--;
	m.unlock();

	sleep(1);
}

int main()
{
	int n = 1000;
	thread th1(funcion1, n), th2(funcion2, n);

	cout << "Proceso principal espera que los hilos terminen\n";
	th1.join();
	th2.join();
	cout << "El hilo principal termina\n";
	cout << "Variable: " << variable << endl;

	exit(0);
}