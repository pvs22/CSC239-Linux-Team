
all: server client

server: msserver.o
	g++ -W -lpthread -g msserver.o -o server

client: mstestclient.o
	g++ -W -lpthread -g mstestclient.o perfstats.o -o testclient

msserver.o: msserver.cpp
	g++ -g -c -W msserver.cpp 

mstestclient.o: mstestclient.cpp perfstats.o
	g++ -g -c -W mstestclient.cpp 

perfstats.o: ../perfstats/perfstats.c
	g++ -g -c -W ../perfstats/perfstats.c

clean:
	rm -rf *.o server testclient core*
