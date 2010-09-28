
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <signal.h>
#include <pthread.h>
#include <netdb.h>
#include "../perfstats/perfstats.h"
#include "perfstatsnetwork.h"

using namespace std;

typedef struct _serverInfo
{
	char* 	serverName;
	int		portNumber;
} serverInfo;

void* getDataFromServers(void *arg);
void parseClientData(int afd);
perf_stats* read_agent_perfstats(int);

int main(int argc, char *argv[])
{
	int 		sfd 	= 0;
	int			portnum = 0;
	int			backlog = 5;
	sockaddr_in my_addr;

	if( argc < 2 )
	{
		cout << "Usage: " << argv[0] << " portNum" << endl;
		return 1;
	}
	portnum = atoi(argv[1]); //get port number from cmd line

	//open socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if( sfd == -1 )
	{
		cerr << "Error opening socket" << endl;
		return 1;
	}

	//bind a address to socket
	memset(&my_addr, 0, sizeof(sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(portnum);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	if( bind(sfd, (const sockaddr *)&my_addr, sizeof(sockaddr_in)) == -1 )
	{
		cerr << "Server : Error binding" << endl;
		return 1;
	}

	//listen for connections on a socket
	if( listen(sfd, backlog) == -1 )
	{
		cerr << "Error listening for connections" << endl;
		return 1;
	}
	
	//accept multiple connections
	signal(SIGCHLD, SIG_IGN); //ignore SIGCHLD message from child. avoids zombie process
	while( 1 )
	{
		int			afd		= 0;
		socklen_t	slen	= 0;
		sockaddr_in	client_addr;
		int			pid		= -1;

		//accept connection 
		slen = sizeof(client_addr);
		afd = accept(sfd, (sockaddr *)&client_addr, &slen);
		if( afd == -1 )
		{
			cerr << "Error accepting connection" << endl;
			continue;
		}

		pid = fork();
		if( pid < 0 )
		{
			cerr << "Error forking" << endl;
			close(afd);
		}
		else if( pid > 0 ) // have child do the work
		{
//			close(sfd); //child doesn't need sfd anymore, so close it
			parseClientData(afd);
			close(afd);
			return 0; // child should exit now
		}
		else //have parent go back to accepting connections
		{
			//parent doesn't need afd, so close it
//			close(afd);
		}
	}

	return 0;
} //main()



void parseClientData(int afd)
{
	//TODO: read data from web server and call appropriate servers to get data
	char buffer[1024];
	memset(buffer, 0, 1024);
	int status = read(afd, buffer, 8);
	cout << "Message from client: " << buffer << endl;
	//TODO: Get list of servers from the web server data
	const int numOfServers = 1;
	string servers[numOfServers];
	servers[0] = "localhost";
	/*servers[1] = "gaia.ecs.csus.edu";*/
	serverInfo* si[numOfServers];
	pthread_t thread[numOfServers];
	for(int i = 0; i < numOfServers; i++)
	{
	  si[i] = (serverInfo*)malloc(sizeof(serverInfo));
	  si[i]->serverName = (char*)malloc(servers[i].size() + 1);
	  strcpy(si[i]->serverName,servers[i].c_str());
	  si[i]->portNumber = 3000;
	  pthread_create(&thread[i], NULL, getDataFromServers, (void*)si[i]);
	}

	for(int i = 0; i < numOfServers; i++){
	  pthread_join(thread[i], NULL);
	  free(si[i]->serverName);
	  free(si[i]);
	}
	
	//TODO: combine information and send to web server

} //parseClientData()



void* getDataFromServers(void *arg)
{
	hostent	*server	= NULL;
	sockaddr_in serv_addr;
	serverInfo *si = (serverInfo*)arg;
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	if( sfd == -1 )
	{
		cerr << "Error opening socket" << endl;
		return NULL;
	}
	//get server
	server = gethostbyname(si->serverName);
	if( server == NULL )
	{
		cerr << "Server " << si->serverName << " not found" << endl;
		return NULL;
	}
	//connect to server
	memset(&serv_addr, 0, sizeof(sockaddr_in));
	serv_addr.sin_family = AF_INET;
	memcpy((void*)&serv_addr.sin_addr.s_addr, (void*)server->h_addr, server->h_length);
	serv_addr.sin_port = htons(si->portNumber);
	if( connect(sfd, (const sockaddr*)&serv_addr, sizeof(sockaddr_in)) == -1 )
	{
		
	  cerr << "Error connecting to server " << si->serverName << endl;
	  return NULL;
	}
	//TODO: send commands to  servers       	
	//TODO: read response from servers 
	printf("Reading perf_stats\n");
	perf_stats* stats = read_agent_perfstats(sfd);
	print_perfstats(stats);
	
} //getDataFromServers()


perf_stats* read_agent_perfstats(int sfd){
  perf_stats* stats = (perf_stats*)malloc(sizeof(perf_stats));
  if (read(sfd, &(stats->platformID), sizeof(char)) == -1){
    printf("Error reading stats->platformID from network socket.\n");
  }
  if (read(sfd, stats->agentName, 16 * sizeof(char)) == -1){
    printf("Error reading stats->agentName from network socket.\n");
  }
  if (read(sfd, &(stats->numCPUs), sizeof(uint64_t)) == -1){
    printf("Error reading &(stats->numCPUs) from network socket.\n");
  }
  stats->cpuStructs = (cpu_info*) malloc(stats->numCPUs * sizeof(cpu_info));
  if (read(sfd, stats->cpuStructs, stats->numCPUs * sizeof(cpu_info)) == -1){
    printf("Error reading stats->cpuStructs from network socket.\n");
  }
  if (read(sfd, &(stats->ctxt), sizeof(uint64_t)) == -1){
    printf("Error reading &(stats->ctxt) from network socket.\n");
  }
  stats->memInfo = (mem_info*) malloc(sizeof(mem_info));
  if (read(sfd, stats->memInfo, sizeof(mem_info)) == -1){
    printf("Error reading &(stats->memInfo) from network socket.\n");
  }
  if (read(sfd, &(stats->numLogDrives), sizeof(uint64_t)) == -1){
    printf("Error reading &(stats->numLogDrives) from network socket.\n");
  }
  if (read(sfd, &(stats->numNetworkAdapters), sizeof(uint64_t)) == -1){
    printf("Error reading &(stats->numNetworkAdapters) from network socket.\n");
  }
  /*stats->networkAdapterStructs = (ns*)malloc(stats->numNetworkAdapters * sizeof(ns));
  if (read(sfd, stats->networkAdapterStructs, sizeof(ns)) == -1){
    printf("Error reading stats->networkAdapterStructs from network socket.\n");
  }*/
  if (read(sfd, &(stats->numProcs), sizeof(uint64_t)) == -1){
    printf("Error reading &(stats->numProcs) from network socket.\n");
    }
  stats->procStructs = (struct procStatNode*) malloc(stats->numProcs * sizeof(struct procStatNode));
  if (read(sfd, stats->procStructs, stats->numProcs * sizeof(struct procStatNode)) == -1){
    printf("Error reading &(stats->procStructs) to network socket.\n");
    }  
  return stats;
}

