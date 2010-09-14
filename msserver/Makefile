
all: server client

server: msserver.o
	g++ -W -lpthread -g msserver.o -o server

client: mstestclient.o
	g++ -W -lpthread -g mstestclient.o readperfstats.o -o testclient

msserver.o: msserver.cpp
	g++ -g -c -W msserver.cpp 

mstestclient.o: mstestclient.cpp readperfstats.o
	g++ -g -c -W mstestclient.cpp 

readperfstats.o: ../readperfstats/readperfstats.c
	g++ -g -c -W ../readperfstats/readperfstats.c

clean:
	rm -rf *.o server testclient core*
