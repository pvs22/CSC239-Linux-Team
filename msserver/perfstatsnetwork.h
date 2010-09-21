#include <inttypes.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <netinet/in.h>
#include <signal.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>

#include "../perfstats/perfstats.h"

#ifndef __PERFSTATSNETWORK__
#define __PERFSTATSNETWORK__

// structure of the data we're sending to the managing server. 
/* |(platform id), 1 byte | (# cpu), 4 bytes| (#cpu struct) * sizeof(cpu struct) | mem_struct | (#logical drives), 4 bytes | (#disk struct) * sizeof(dist struct) | (#network interfaces) * sizeof(network struct) | # process (4 bytes) | # process * sizeof(proc struct) 
 */
typedef struct
{
  char platformID;
  char agentName[16];
  uint64_t numCPUs;
  void* cpuStructs;
  uint64_t ctxt; // total number of context switches.
  mem_info* memInfo;
  uint64_t numLogDrives;
  uint64_t numNetworkAdapters;
  void* networkAdapterStructs;
  uint64_t numProcs;
  void* procStructs;
} perf_stats;

int copy_platformID(perf_stats*);
int copy_name(perf_stats*);
int copy_mem_stats(perf_stats*);
int copy_cpu_stats(perf_stats*);
int copy_logdrive_stats(perf_stats*);
int copy_networkadapter_stats(perf_stats*);
int copy_process_stats(perf_stats*);

int print_perfstats(perf_stats*);
int print_platformID(perf_stats*);
int print_name(perf_stats*);
int print_mem_stats(perf_stats*);
int print_cpu_stats(perf_stats*);
int print_logdrive_stats(perf_stats*);
int print_networkadapter_stats(perf_stats*);
int print_process_stats(perf_stats*);


int send_perfstats(int afd, perf_stats*); 
#endif
