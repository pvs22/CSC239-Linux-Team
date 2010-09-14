#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
// structure to hold memory (RAM) information.
typedef struct {
    int free;
    int used;
    int total;
    float util;
    int pgin;
    int pgout;
    int swpin;
    int swpout;
} mem_info;

// structure to hold network information.
typedef struct 
{
  char networkName[256];
  int bytesRec;
  int bytesSent;
  int errs;
  int colls;
} ns;



void readmeminfo();
void readcpuinfo();
void readdiskioinfo();


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

int readvmstats(mem_info* minfo)
{
    FILE *f;
    char buffer[1024];
    char str[] = "pgpgin";
    
    if(f = fopen("/proc/vmstat", "r"))
    {
        while(fgets(buffer, sizeof(buffer), f))
        {
            if(strstr(buffer, str))
            {
                fseek(f, -(long)strlen(buffer), SEEK_CUR);
                if(fscanf(f, "pgpgin %d pgpgout %d pswpin %d pswpout %d", 
                          &(minfo->pgin), &(minfo->pgout), &(minfo->swpin), &(minfo->swpout)) == 0)
                {
                    fprintf(stderr, "Error: Cannot read info from /proc/vmstat\n");
                    fclose(f);
                    return 1;
                }
                else 
                {   printf("----- VM stats ------\n");
                    printf("Number of pages paged in = %d\n", minfo->pgin);
                    printf("Number of pages paged out = %d\n", minfo->pgout);
                    printf("Number of pages swaped in = %d\n", minfo->swpin);
                    printf("Number of pages swaped out = %d\n", minfo->swpout);					
                }
            }
        }
    }
    else
    {
        fprintf(stderr, "Error Cannot open /proc/vmstat");
        return 1;
    }
    
    return 0;
}



void readmeminfo()
{
        FILE* f;
	mem_info meminfo;
        int memTotal, memUsed, memFree;
        float util;

        if (f = fopen("/proc/meminfo", "r"))
        {
	  if (fscanf(f, "MemTotal: %d kB MemFree: %d kb", &meminfo.total, &meminfo.free) == 0) {
	    fclose(f);
	    fprintf(stderr, "Error : Can not read MemTotal and MemFree from /proc/meminfo\n");
	  } else {  
	  
	    meminfo.used = memTotal - memFree;
	    meminfo.util = (float) memUsed/(1.0*memTotal)*100;
	    
	    printf ("--- Memory Stats ---\n");
	    printf ("Free Memory = %d KB\n", meminfo.free);
	    printf ("Used Memory = %d KB\n", meminfo.used);
	    printf ("Total Memory = %d KB\n", meminfo.total);
	    printf ("Memory Utilization = %5.2f%%\n", meminfo.util);
	    fclose(f);
	  }
        }
        else
        {
	  fprintf(stderr, "Error : Can not open /proc/meminfo");
        }
	readvmstats(&meminfo);

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
