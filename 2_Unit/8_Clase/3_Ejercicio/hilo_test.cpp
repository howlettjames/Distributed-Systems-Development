#include <iostream>
#include <unistd.h>
#include <thread>
#include <atomic>

using namespace std;

// int variable = 0;
atomic<int> var_glo(0);

// void funcion1(int n)
void funcion1(int n, atomic<int>& variable)
{
	for(int i = 0; i < n; i++)
		variable++;

	sleep(1);
}

// void funcion2(int n)
void funcion2(int n, atomic<int>& variable)
{
	for(int i = 0; i < n; i++)
		variable--;

	sleep(1);
}

int main()
{
	int n = 1000;
	atomic<int> var_local(0);
	thread th1(funcion1, n, ref(var_local)), th2(funcion2, n, ref(var_local));

	cout << "Proceso principal espera que los hilos terminen\n";
	th1.join();
	th2.join();
	cout << "El hilo principal termina\n";
	// cout << "Variable: " << var_glo << endl;
	cout << "Variable: " << var_local << endl;

	exit(0);
}