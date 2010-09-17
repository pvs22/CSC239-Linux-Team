
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include "../perfstats/perfstats.h"

using namespace std;

int printmemstats (char* buffer);
int printcpustats(char* buffer);


// structure of the data we're sending to the managing server. 
/* |(platform id), 1 byte | (# cpu), 4 bytes| (#cpu struct) * sizeof(cpu struct) | mem_struct | (#logical drives), 4 bytes | (#disk struct) * sizeof(dist struct) | (#network interfaces) * sizeof(network struct) | # process (4 bytes) | # process * sizeof(proc struct) 
 */
typedef struct
{
  char platformId;
  int numCPUs;
  void* cpuStructs;
  mem_info* memInfo;
  int numLogDrives;
  int numNetworkAdapters;
  void* networkAdapterStructs;
  int numProcs;
  void* procStructs;
} perf_stats;

int copy_platformId(perf_stats* stats);
int copy_mem_stats(perf_stats* stats);
int copy_cpu_stats(perf_stats* stats);
int copy_logdrive_stats(perf_stats* stats);
int copy_networkadapter_stats(perf_stats* stats);
int copy_process_stats(perf_stats* stats);
int print_platformId(perf_stats* stats);
int print_mem_stats(perf_stats* stats);
int print_cpu_stats(perf_stats* stats);
int print_logdrive_stats(perf_stats* stats);
int print_networkadapter_stats(perf_stats* stats);
int print_process_stats(perf_stats* stats);


int main (int argc, char *argv[])
{
  
  perf_stats stats;
  copy_platformId(&stats);
  copy_mem_stats(&stats);
  copy_cpu_stats(&stats);
  copy_logdrive_stats(&stats);
  copy_networkadapter_stats(&stats);
  copy_process_stats(&stats);
  

  
  print_platformId(&stats);
  print_mem_stats(&stats);
  print_cpu_stats(&stats);
  print_logdrive_stats(&stats);
  print_networkadapter_stats(&stats);
  print_process_stats(&stats);
  
  return 0;
}


int copy_platformId(perf_stats* stats){
  stats->platformId = 'L';
  return 0;
}

int copy_mem_stats(perf_stats* stats){
  mem_info* m = (mem_info*)malloc(sizeof(mem_info));
  if (read_meminfo(m)) {
    printf("Error reading memory information.\n");
    return -1;
  }
  stats->memInfo = m;
  return 0;
}

int copy_cpu_stats(perf_stats* stats){
  // For now we only handle 1 cpu. This needs to be fixed in the future.
  int numCPUs = 1;
  stats->numCPUs = numCPUs;
  cpu_info* cpuInfo = (cpu_info*)malloc(sizeof(cpu_info) * numCPUs);
  if (read_cpuinfo(cpuInfo) != 0) {
    printf("Error reading cpu information.\n");
    return -1;
  }
  /* TODO, copy cpuInfo to the stats variable. */
  
  return 0;
}

int copy_logdrive_stats(perf_stats* stats){
  /* TODO */
  return 0;
}

int copy_networkadapter_stats(perf_stats* stats){
  ns* nsStats = NULL;
  int numAdapters = 0;
  if (read_networkinfo(nsStats, &numAdapters) != 0){
    printf("Error reading network stats.\n");
    return -1;
  }

  stats->numNetworkAdapters = numAdapters;
  stats->networkAdapterStructs = nsStats;
  return 0;
}

int copy_process_stats(perf_stats* stats){
  /* TODO */
  return 0;
}


int print_platformId(perf_stats* stats){
  printf("Platform Id:%c\n", stats->platformId);
  return 0;
}

int print_mem_stats(perf_stats* stats){
  /* TODO */
  return 0;
}

int print_cpu_stats(perf_stats* stats){
  /* TODO */
  return 0;
}

int print_logdrive_stats(perf_stats* stats){
  /* TODO */
  return 0;
}

int print_networkadapter_stats(perf_stats* stats){
  /* TODO */
  return 0;
}

int print_process_stats(perf_stats* stats){
  /* TODO */
  return 0;
}
