#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main()
{
        FILE* f;
        int memTotal, memUsed, memFree;
        float util;

        if (f = fopen("/proc/meminfo", "r"))
        {
	  if (fscanf(f, "MemTotal: %d kB MemFree: %d kb", &memTotal, &memFree) == 0) {
	    fclose(f);
	    fprintf(stderr, "Error : Can not read MemTotal and MemFree from /proc/meminfo\n");
	  } else {  
	  
	    memUsed = memTotal - memFree;
	    util = (float) memUsed/(1.0*memTotal)*100;
	    
	    printf ("--- Memory Stats ---\n");
	    printf ("Free Memory = %d KB\n", memFree);
	    printf ("Used Memory = %d KB\n", memUsed);
	    printf ("Total Memory = %d KB\n", memTotal);
	    printf ("Memory Utilization = %5.2f%%\n", util);
	    fclose(f);
	  }
        }
        else
        {
	  fprintf(stderr, "Error : Can not open /proc/meminfo");
        }
}

