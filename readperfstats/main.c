#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "readperfstats.h"

int main()
{
  mem_info meminfo;
  if (readmeminfo(&meminfo) != 0) {
    printf("Error reading memory information.\n");
    return -1;
  }
  printf ("--- Memory Stats ---\n");
  printf ("Free Memory = %d KB\n", meminfo.free);
  printf ("Used Memory = %d KB\n", meminfo.used);
  printf ("Total Memory = %d KB\n", meminfo.total);
  printf ("Memory Utilization = %5.2f%%\n", meminfo.util);
  printf("----- VM stats ------\n");
  printf("Number of pages paged in = %d\n", meminfo.pgin);
  printf("Number of pages paged out = %d\n", meminfo.pgout);
  printf("Number of pages swaped in = %d\n", meminfo.swpin);
  printf("Number of pages swaped out = %d\n", meminfo.swpout);					

}
