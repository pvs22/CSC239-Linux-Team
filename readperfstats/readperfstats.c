#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void readmeminfo();
void readcpuinfo();
void readdiskioinfo();
void readnetworkinfo();
void readprocessinfo();

int main()
{
  readmeminfo();
  readcpuinfo();
  readdiskioinfo();
  readnetworkinfo();
  readprocessinfo();
}

void readmeminfo()
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


void readcpuinfo(){
        FILE* f;
        /* add in variables used to display cpu info */

        if (f = fopen("/proc/***", "r"))
        {
	  /* use fscanf to the read data */
	  if (fscanf(f, "***format_string***" /* variable list */) == 0) {
	    fclose(f);
	    fprintf(stderr, "Error : Can not read from /proc/***\n");
	  } else {  
	    printf ("--- CPU Stats ---\n");
	    /* print cpu stats */
	    fclose(f);
	  }
        }
        else
        {
	  fprintf(stderr, "Error : Can not open /proc/***");
        }

};
void readdiskioinfo(){
        FILE* f;
        /* add in variables used to display disk io info */

        if (f = fopen("/proc/***", "r"))
        {
	  /* use fscanf to the read data */
	  if (fscanf(f, "***format_string***" /*, variable list */) == 0) {
	    fclose(f);
	    fprintf(stderr, "Error : Can not read from /proc/***\n");
	  } else {  
	    printf ("--- Disk IO Stats ---\n");
	    /* print disk io stats */
	    fclose(f);
	  }
        }
        else
        {
	  fprintf(stderr, "Error : Can not open /proc/***");
        }


};
void readnetworkinfo(){
        FILE* f;
        /* add in variables used to display network info */

        if (f = fopen("/proc/***", "r"))
        {
	  /* use fscanf to the read data */
	  if (fscanf(f, "***format_string***" /*, variable list */) == 0) {
	    fclose(f);
	    fprintf(stderr, "Error : Can not read from /proc/***\n");
	  } else {  
	    printf ("--- Network Stats ---\n");
	    /* print cpu stats */
	    fclose(f);
	  }
        }
        else
        {
	  fprintf(stderr, "Error : Can not open /proc/***");
        }


};
void readprocessinfo(){
        FILE* f;
        /* add in variables used to display process info */

        if (f = fopen("/proc/***", "r"))
        {
	    printf ("--- Process Stats ---\n");
	    /* print process stats */
	    fclose(f);
        }
        else
        {
	  fprintf(stderr, "Error : Can not open /proc/***");
        }


};
