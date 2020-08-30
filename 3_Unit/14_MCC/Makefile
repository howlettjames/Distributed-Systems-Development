emisor: emisor.cpp SocketMulticast.o
	g++ emisor.cpp SocketMulticast.o PaqueteDatagrama.o -o emisor -std=c++11
receptor: receptor.cpp SocketMulticast.o
	g++ receptor.cpp SocketMulticast.o PaqueteDatagrama.o -o receptor -std=c++11
SocketMulticast.o: SocketMulticast.cpp PaqueteDatagrama.o SocketMulticast.h
	g++ SocketMulticast.cpp -c -std=c++11
PaqueteDatagrama.o: PaqueteDatagrama.cpp PaqueteDatagrama.h
	g++ PaqueteDatagrama.cpp -c -std=c++11
