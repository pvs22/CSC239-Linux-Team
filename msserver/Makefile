
all: server client agent

server: msserver.o perfstats.o perfstatsnetwork.o
	g++ -W -lpthread -g msserver.o perfstats.o perfstatsnetwork.o -o server

client: mstestclient.o perfstats.o
	g++ -W -lpthread -g mstestclient.o perfstats.o -o testclient

agent: agent.o perfstats.o perfstatsnetwork.o
	g++ -W -lpthread -g agent.o perfstats.o perfstatsnetwork.o -o agent

agent.o: agent.cpp 
	g++ -g -c -W agent.cpp

msserver.o: msserver.cpp
	g++ -g -c -W msserver.cpp 

mstestclient.o: mstestclient.cpp
	g++ -g -c -W mstestclient.cpp 

perfstatsnetwork.o: perfstatsnetwork.c
	g++ -g -c -W perfstatsnetwork.c

perfstats.o: ../perfstats/perfstats.c
	g++ -g -c -W ../perfstats/perfstats.c

clean:
	rm -rf *.o server testclient agent core*
