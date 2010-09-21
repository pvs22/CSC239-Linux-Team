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
int connect_to_msserver();
		    
int main (int argc, char *argv[])
{
  
  perf_stats stats;
  copy_platformID(&stats);
  copy_name(&stats);
  copy_mem_stats(&stats);
  copy_cpu_stats(&stats);
  copy_logdrive_stats(&stats);
  copy_networkadapter_stats(&stats);
  copy_process_stats(&stats);
  int sfd = -1;
  sfd = connect_to_msserver();
  if (sfd < 0){
    return sfd;
  }
  for (; ;) {
    int			afd		= 0;
    socklen_t	slen	= 0;
    sockaddr_in	client_addr;
    int			pid		= -1;
    
    //accept connection 
    slen = sizeof(client_addr);
    afd = accept(sfd, (sockaddr *)&client_addr, &slen);
    if( afd == -1 ){
      cerr << "Error accepting connection" << endl;
      return -1;
    }
    cout << "connection accepted" << endl;
    
    
    send_perfstats(afd, &stats);  
    shutdown(afd, SHUT_RDWR);
    close(afd);
  }
  close(sfd);
  return 0;
}

int connect_to_msserver(){
  	int 	sfd		= 0;
	hostent	*server = NULL;
	sockaddr_in	serv_addr;
	int portNum = 3000;
	int			backlog = 5;
	sockaddr_in my_addr;


	//open socket
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if( sfd == -1 )
	{
		cerr << "Error opening socket" << endl;
		return -1;
	}
	cout << "socket opened" << endl;
	//bind a address to socket
	memset(&my_addr, 0, sizeof(sockaddr_in));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(portNum);
	my_addr.sin_addr.s_addr = INADDR_ANY;
	if( bind(sfd, (const sockaddr *)&my_addr, sizeof(sockaddr_in)) == -1 )
	{
		cerr << "Error binding" << endl;
		return -1;
	}
	cout << "socket bound" << endl;

	//listen for connections on a socket
	if( listen(sfd, backlog) == -1 )
	{
		cerr << "Error listening for connections" << endl;
		return -1;
	}

	cout << "socket listened" << endl;

	
	return sfd;
}
