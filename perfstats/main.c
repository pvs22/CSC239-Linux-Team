#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "perfstats.h"

int main()
{
  mem_info meminfo;
  if (read_meminfo(&meminfo) != 0) {
    printf("Error reading memory information.\n");
    return -1;
  }
  printf ("--- Memory Stats ---\n");
  printf ("Free Memory = %llu KB\n", meminfo.free);
  printf ("Used Memory = %llu KB\n", meminfo.used);
  printf ("Total Memory = %llu KB\n", meminfo.total);
  printf ("Memory Utilization = %5.2f%%\n", meminfo.util);
  printf("----- VM stats ------\n");
  printf("Number of pages paged in = %llu\n", meminfo.pgin);
  printf("Number of pages paged out = %llu\n", meminfo.pgout);
  printf("Number of pages swaped in = %llu\n", meminfo.swpin);
  printf("Number of pages swaped out = %llu\n", meminfo.swpout);					
  struct procStats stats;
  read_procstats(&stats);
  cpu_info* cpus = NULL;
  uint64_t numCPUs = 0;
  cpus = read_cpuinfo(&numCPUs);
  if (cpus == NULL){
    printf("Error reading cpu_info\n");
    exit(-1);
  }
  uint64_t i = 0;
  for (i = 0; i < numCPUs; i++){

    printf("CPU Num : %llu\n", cpus[i].cpuNum);
    printf("User : %llu\n", cpus[i].user);
    printf("System : %llu\n", cpus[i].system);
    printf("Idle : %llu\n\n", cpus[i].idle);
  }
  return 0; 
}
