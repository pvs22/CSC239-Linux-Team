#include "perfstatsnetwork.h"

int print_perfstats(perf_stats* stats){
  if (print_platformID(stats) != 0) {
    printf("Error printing platformID\n");
  }
  if (print_name(stats) != 0) {
    printf("Error printing name\n");
  }
  if (print_mem_stats(stats) != 0) {
    printf("Error printing mem_stats\n");
  }
  if (print_cpu_stats(stats) != 0) {
    printf("Error printing cpu_stats\n");
  }
  if (print_logdrive_stats(stats) != 0) {
    printf("Error printing logdrive_stats\n");
  }
  if (print_networkadapter_stats(stats) != 0) {
    printf("Error printing networkadapter_stats\n");
  }
  if (print_process_stats(stats) != 0) {
    printf("Error printing process_stats\n");
  }
  
}


int send_perfstats(int afd, perf_stats* stats){
  if (write(afd, &(stats->platformID), sizeof(char)) == -1){
    printf("Error writing stats->platformID to network socket.\n");
  }
  if (write(afd, stats->agentName, 16 * sizeof(char)) == -1){
    printf("Error writing stats->agentName to network socket.\n");
  }
  if (write(afd, &(stats->numCPUs), sizeof(uint64_t)) == -1){
    printf("Error writing &(stats->numCPUs) to network socket.\n");
  }
  if (write(afd, stats->cpuStructs, stats->numCPUs * sizeof(cpu_info)) == -1){
    printf("Error writing stats->cpuStructs to network socket.\n");
  }
  if (write(afd, &(stats->ctxt), sizeof(uint64_t)) == -1){
    printf("Error writing &(stats->ctxt) to network socket.\n");
  }
  if (write(afd, &(stats->memInfo), sizeof(mem_info)) == -1){
    printf("Error writing &(stats->memInfo) to network socket.\n");
  }
  if (write(afd, &(stats->numLogDrives), sizeof(uint64_t)) == -1){
    printf("Error writing &(stats->numLogDrives) to network socket.\n");
  }
  if (write(afd, &(stats->numNetworkAdapters), sizeof(uint64_t)) == -1){
    printf("Error writing &(stats->numNetworkAdapters) to network socket.\n");
  }
  /*  if (write(afd, stats->networkAdapterStructs, stats->numNetworkAdapters * sizeof(ns)) == -1){
    printf("Error writing stats->networkAdapterStructs to network socket.\n");
  }
  if (write(afd, &(stats->numProcs), sizeof(uint64_t)) == -1){
    printf("Error writing &(stats->numProcs) to network socket.\n");
    }*/  
}



int copy_platformID(perf_stats* stats){
  stats->platformID = 'L';
  return 0;
}

int copy_name(perf_stats* stats){
  char agentName[16] = "agent1         ";
  for (int i = 0; i < 16; i++){
    stats->agentName[i] = agentName[i];
  }
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
  uint64_t numCPUs = 1;
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
  uint64_t numAdapters = 0;
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


int print_platformID(perf_stats* stats){
  printf("Platform Id:%c\n", stats->platformID);
  return 0;
}

int print_name(perf_stats* stats){
  printf("Name:%s\n", stats->agentName);
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
