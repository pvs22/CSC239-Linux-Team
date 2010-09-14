#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void readmeminfo();
void readcpuinfo();
void readdiskioinfo();


// structure to hold network information.
typedef struct 
{
  char networkName[256];
  int bytesRec;
  int bytesSent;
  int errs;
  int colls;
} ns;

void readnetworkinfo();
void readprocessinfo();



int main()
{
  readmeminfo();
  //readcpuinfo();
  //readdiskioinfo();
  readnetworkinfo();
  //readprocessinfo();
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

void readnetworkinfo()
{
	FILE* f;
	int a,j;
	int total_colls = 0 ;
	int total_bytesrec = 0;
	int total_bytesent = 0;
	int total_errs = 0;
	int i = 0;
	char new[25];
	int buffer_size = 1024;
	char buffer[buffer_size];
	int fscanf_value;

	ns b[3];

        if (f = fopen("/proc/net/dev", "r")) {
	  fgets(buffer, buffer_size, f);
	  fgets(buffer, buffer_size, f);
	  fscanf_value = fscanf(f, " %[^:]: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", b[i].networkName, &b[i].bytesRec, &b[i].errs, &a, &a, &a, &a, &a, &a, &b[i].bytesSent, &b[i].errs, &a, &a, &b[i].colls, &a, &a, &a);
	  while (!feof(f))
	    {
	      if (fscanf_value== 0){
		fclose(f);
		fprintf(stderr, "Error : Can not read network data from /proc/net/dev\n");
		return;
	      } else {
		printf ("--- Network Stats ---\n");
		printf("Network Name		= %s \n", b[i].networkName);
		printf ("Bytes Received		= %d \n", b[i].bytesRec);
		printf ("Bytes Sent 		= %d \n", b[i].bytesSent);
		printf ("Error			= %d \n", b[i].errs);
		printf ("Collisions		= %d \n", b[i].colls);
		
		total_bytesrec = total_bytesrec + b[i].bytesRec;
		total_bytesent = total_bytesent + b[i].bytesSent;
		total_errs	 = total_errs + b[i].errs;
		total_colls	 = total_colls + b[i].colls;
		i++;
	      }
	      fscanf_value = fscanf(f, " %[^:]: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", b[i].networkName, &b[i].bytesRec, &b[i].errs, &a, &a, &a, &a, &a, &a, &b[i].bytesSent,&b[i].errs,&a,&a,&b[i].colls, &a, &a, &a);
	    }
	  printf("-----Overall Network Stats---\n");
	  printf("Total bytes received 	= %d \n",total_bytesrec);
	  printf("Total bytes sent 	= %d \n",total_bytesent);
	  printf("Total errors		= %d \n",total_errs);
	  printf("Total collision 	= %d \n",total_colls);
	} else {
	  fprintf(stderr, "Error : Can not open /proc/meminfo");
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
