
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

int main(int argc, char *argv[])
{
	int 	sfd		= 0;
	hostent	*server = NULL;
	sockaddr_in	serv_addr;

	if( argc < 4 )
	{
		cout << "Usage: " << argv[0] << " server portnum message" << endl;
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

	int status = write(sfd, argv[3], strlen(argv[3]));
	if( status == -1 )
	{
		cerr << "Error writing to server" << endl;
		return 1;
	}

	return 0;
}
