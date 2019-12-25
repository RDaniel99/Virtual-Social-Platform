all:
	g++ -g text.cpp server.cpp -o serverVirtualSoc
	g++ -g text.cpp client.cpp -o clientVirtualSoc
