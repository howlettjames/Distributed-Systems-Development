
cliente: cliente.cpp SocketDatagrama.o
	g++ cliente.cpp SocketDatagrama.o PaqueteDatagrama.o -o cliente
servidor: servidor.cpp SocketDatagrama.o
	g++ servidor.cpp SocketDatagrama.o PaqueteDatagrama.o -o servidor
SocketDatagrama.o: SocketDatagrama.cpp PaqueteDatagrama.o SocketDatagrama.h
	g++ SocketDatagrama.cpp -c
PaqueteDatagrama.o: PaqueteDatagrama.cpp PaqueteDatagrama.h
	g++ PaqueteDatagrama.cpp -c
