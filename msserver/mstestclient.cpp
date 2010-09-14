
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include "../readperfstats/readperfstats.h"

using namespace std;
void printmemstats (char* buffer);

int main (int argc, char *argv[])
{
	int 	sfd		= 0;
	hostent	*server = NULL;
	sockaddr_in	serv_addr;

	if( argc < 2 )
	{
		cout << "Usage: " << argv[0] << " server portnum" << endl;
		return 0;
	}

	//open socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if( sfd == -1 )
	{
		cerr << "Error opening socket" << endl;
		return 1;
	}

	//get server
	server = gethostbyname(argv[1]);
	if( server == NULL )
	{
		cerr << "Server " << argv[1] << " not found!" << endl;
		return 1;
	}

	//connect to server
	memset(&serv_addr, 0, sizeof(sockaddr_in));
	serv_addr.sin_family = AF_INET;
	memcpy((void*)&serv_addr.sin_addr.s_addr, (void*)server->h_addr, server->h_length);
	serv_addr.sin_port = htons(atoi(argv[2]));
	if( connect(sfd, (const sockaddr*)&serv_addr, sizeof(sockaddr_in)) == -1 )
	{
		cerr << "Error connecting to server" << endl;
		return 1;
	}
	
	char msg[2048];
	printmemstats(msg);
	int status = write(sfd, msg, strlen(msg));
	if( status == -1 )
	{
		cerr << "Error writing to server" << endl;
		return 1;
	}

	return 0;
}

void printmemstats (char* buffer){
  mem_info meminfo;
  if (readmeminfo(&meminfo) != 0){
    printf("Could not read memory information\n");
    return;
  }
  
  sprintf (buffer, "--- Memory Stats ---\nFree Memory = %d KB\nUsed Memory = %d KB\nTotal Memory = %d KB\nMemory Utilization = %5.2f%%\nNumber of pages paged in = %d\nNumber of pages paged out = %d\nNumber of pages swaped in = %d\nNumber of pages swaped out = %d\n", 
	   meminfo.free, meminfo.used, meminfo.total, meminfo.util, meminfo.pgin, meminfo.pgout, meminfo.swpin, meminfo.swpout);
}
