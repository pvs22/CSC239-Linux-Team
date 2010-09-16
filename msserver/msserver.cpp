
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netinet/in.h>
#include <signal.h>
#include <pthread.h>
#include <netdb.h>

using namespace std;

typedef struct _serverInfo
{
	string 	serverName;
	int		portNumber;
} serverInfo;

void* getDataFromServers(void *arg);
void parseClientData(int afd);

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
		cerr << "Error binding" << endl;
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
			close(sfd); //child doesn't need sfd anymore, so close it
			parseClientData(afd);
			return 0; // child should exit now
		}
		else //have parent go back to accepting connections
		{
			//parent doesn't need afd, so close it
			close(afd);
		}
	}

	return 0;
} //main()



void parseClientData(int afd)
{
	//TODO: read data from web server and call appropriate servers to get data
	char buffer[1024];
	memset(buffer, 0, 1024);
	int status = read(afd, buffer, 1024);
	cout << "Message from client: " << buffer << endl;

	//TODO: Get list of servers from the web server data
	const int numOfServers = 2;
	string servers[numOfServers];
	servers[0] = "localhost";
	servers[1] = "gaia.ecs.csus.edu";

	pthread_t thread[numOfServers];
	for(int i = 0; i < numOfServers; i++)
	{
		serverInfo si;
		si.serverName = servers[i];
		si.portNumber = 3000;
		pthread_create(&thread[i], NULL, getDataFromServers, (void*)&si);
	}

	for(int i = 0; i < numOfServers; i++)
		pthread_join(thread[i], NULL);

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
	server = gethostbyname(si->serverName.c_str());
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
		
		cerr << "Error connecting to server" << endl;
		return NULL;
	}

	//TODO: send commands to  servers

	//TODO: read response from servers 
	//int status = read(sfd, buffer, bufSize);
	
} //getDataFromServers()



