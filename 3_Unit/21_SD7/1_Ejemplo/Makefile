cliente: cliente.cpp SocketDatagrama.o
	g++ cliente.cpp SocketDatagrama.o PaqueteDatagrama.o -o cliente -std=c++11
SocketDatagrama.o: SocketDatagrama.cpp PaqueteDatagrama.o SocketDatagrama.h
	g++ SocketDatagrama.cpp -c -std=c++11
PaqueteDatagrama.o: PaqueteDatagrama.cpp PaqueteDatagrama.h
	g++ PaqueteDatagrama.cpp -c -std=c++11

clean:
	rm -f *.o $(PROYECTO)
