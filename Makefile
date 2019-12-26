all:
	g++ -g text.cpp database.cpp server.cpp -l sqlite3 -o serverVirtualSoc
	g++ -g text.cpp client.cpp -o clientVirtualSoc