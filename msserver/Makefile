
all: server client

server: msserver.o
	g++ -W -lpthread -g msserver.o -o server

client: mstestclient.o
	g++ -W -lpthread -g mstestclient.o -o testclient

msserver.o: msserver.cpp
	g++ -g -c -W msserver.cpp

mstestclient.o: mstestclient.cpp
	g++ -g -c -W mstestclient.cpp

clean:
	rm -rf *.o server testclient core*
