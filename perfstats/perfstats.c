#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include "perfstats.h"

int read_vmstats(mem_info* minfo)
{
    FILE *f;
    char buffer[1024];
    char str[] = "pgpgin";
    
    int pgin, pgout, swpin, swpout;

    if(f = fopen("/proc/vmstat", "r"))
    {
        while(fgets(buffer, sizeof(buffer), f))
        {
            if(strstr(buffer, str))
            {
                fseek(f, -(long)strlen(buffer), SEEK_CUR);
                if(fscanf(f, "pgpgin %d pgpgout %d pswpin %d pswpout %d", 
                          &pgin, &pgout, &swpin, &swpout) == 0)
                {
                    fprintf(stderr, "Error: Cannot read info from /proc/vmstat\n");
                    fclose(f);
                    return 1;
                }
		minfo->pgin = pgin;
		minfo->pgout = pgout;
		minfo->swpin = swpin;
		minfo->swpout = swpout;
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



int read_meminfo(mem_info* meminfo)
{
        FILE* f;
	int mtotal, mfree;
        if (f = fopen("/proc/meminfo", "r")) {
	  if (fscanf(f, "MemTotal: %d kB MemFree: %d kb", &mtotal, &mfree) == 0) {
	    fclose(f);
	    fprintf(stderr, "Error : Can not read MemTotal and MemFree from /proc/meminfo\n");
	    return -1;
	  } else {  
	    meminfo->total = mtotal;
	    meminfo->free = mfree;
	    meminfo->used = meminfo->total - meminfo->free;
	    meminfo->util = (float) meminfo->used/(1.0*meminfo->total)*100;	    
	    fclose(f);
	    }
	} else {
	  fprintf(stderr, "Error : Can not open /proc/meminfo");
	  return -1;
	}
	read_vmstats(meminfo);
	return 0;
}



int read_cpuinfo(cpu_info* cinfo)
{
  char str[4];
  char newstr[4];
  FILE *fp;
  long int user,sys, nice, idle, val3, val4, val5, val6;
  if(fp = fopen("/proc/stat", "r")){
    if(fscanf(fp, "%s %ld %ld %ld %ld %ld %ld %ld %ld", str, &user, &nice, &sys, &idle, &val3, &val4, &val5, &val6)==0){
      fclose(fp);
      fprintf(stderr, "Error : Can not read from /proc/stat\n");
    }else{
      if(strcmp(str, "cpu")==0){
	cinfo->user = user + nice;
	cinfo->system = sys;
	cinfo->idle = idle;
      }
      fclose(fp);
    }
  }else{
    fprintf(stderr, "Error : Can not open /proc/stat");
    return -1;
  }
  return 0;
}



void read_diskioinfo(){
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

int read_networkinfo(ns* nsStats, uint64_t* numAdapters)
{
	FILE* f;
	int a,j;
	uint64_t total_colls = 0 ;
	uint64_t total_bytesrec = 0;
	uint64_t total_bytesent = 0;
	uint64_t total_errs = 0;
	uint64_t i = 0;
	uint64_t buffer_size = 1024;
	char buffer[buffer_size];
	uint64_t fscanf_value;
	uint64_t MAX_NUM_ADAPTERS = 128;
	ns* b = (ns*)malloc(sizeof(ns) * MAX_NUM_ADAPTERS);
	nsStats = b;

        if (f = fopen("/proc/net/dev", "r")) {
	  fgets(buffer, buffer_size, f);
	  fgets(buffer, buffer_size, f);
	  int bytesRec, errs, bytesSent, colls;
	  
	  fscanf_value = fscanf(f, " %[^:]: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", b[i].networkName, &bytesRec, &errs, &a, &a, &a, &a, &a, &a, &bytesSent, &errs, &a, &a, &colls, &a, &a, &a);
	  b[i].bytesRec = bytesRec;
	  b[i].errs = errs;
	  b[i].bytesSent = bytesSent;
	  b[i].colls = colls;
	  while (!feof(f))
	    {
	      if (fscanf_value== 0){
		fclose(f);
		fprintf(stderr, "Error : Can not read network data from /proc/net/dev.\n");
		return -1;
	      } else {
		total_bytesrec = total_bytesrec + b[i].bytesRec;
		total_bytesent = total_bytesent + b[i].bytesSent;
		total_errs	 = total_errs + b[i].errs;
		total_colls	 = total_colls + b[i].colls;
		i++;
	      }
	      fscanf_value = fscanf(f, " %[^:]: %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d", b[i].networkName, &bytesRec, &errs, &a, &a, &a, &a, &a, &a, &bytesSent, &errs,&a,&a,&colls, &a, &a, &a);
	      
	      b[i].bytesRec = bytesRec;
	      b[i].errs = errs;
	      b[i].bytesSent = bytesSent;
	      b[i].colls = colls;
	      
	    }
	  *numAdapters = i + 1; 
	} else {
	  fprintf(stderr, "Error : Can not open /proc/meminfo.\n");
	  return -1;
	}	
	return 0;
};
 
int read_procstats (struct procStats *p)
{
        getProcStats (p, 0);
	return 0;
}

void getProcStats (struct procStats *procPtr, int iter)

{
        struct procStatNode *nodePtr;
        DIR *d;
        struct dirent *ent;
        FILE *f;
        int ppid, pgrp, session, tty_nr, tpgid;
        unsigned flags;
        unsigned long minflt, majflt, cmajflt, utime, stime, cutime, cstime;
        long int priority, nice, num_thr, itrealv;
        unsigned long startTime;
        char fname[100];
 
        procPtr->procTotal = 0;
        procPtr->procRun = 0;
        procPtr->procSleep = 0;
        procPtr->procDSleep = 0;
        procPtr->procZombie = 0;
        procPtr->procTrace = 0;
        procPtr->procPaging = 0;
        procPtr->head = NULL;
 
        if (d = opendir ("/proc"))
        {
                while (ent = readdir(d))
                {
                        if (is_process(ent->d_name))
                        {
                                if (procPtr->head == NULL)
                                {
                                        procPtr->head = (struct procStatNode *) malloc (sizeof(struct procStatNode));;
                                        nodePtr = procPtr->head;
                                } else {
                                        nodePtr->nxtProc = (struct procStatNode *) malloc (sizeof(struct procStatNode));
                                        nodePtr = nodePtr->nxtProc;
                                }
                                nodePtr->nxtProc = NULL;
 
                                sprintf (fname, "/proc/%s/stat", ent->d_name);
                                if (f = fopen(fname, "r"))
                                {
                                        fscanf (f, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %llu", &(nodePtr->pid), &(nodePtr->cmd), &(nodePtr->state), &ppid, &pgrp, &session, &tty_nr, &tpgid, &flags, &minflt, &majflt, &cmajflt, &utime, &stime, &cutime, &cstime, &priority, &nice, &num_thr, &itrealv, &startTime);
                                        fclose(f);
 
                                        // Calculate CPU utilization here
                                        // nodePtr->cpuUtil = (utime + stime)/100.0;
                                        nodePtr->cpuTime = (utime + stime);
                                        // Assumption startTime/100 = wall time in secs
                                        // getWallTime(&(nodePtr->wt), startTime);
                                        nodePtr->wallTime = startTime/100;
                                } else
                                        printf ("Error! %s file not found\n", fname);
 
                                sprintf (fname, "/proc/%s/statm", ent->d_name);
                                if (f = fopen(fname, "r"))
                                {
                                        fscanf (f, "%d %d %d", &(nodePtr->sizeTotal), &(nodePtr->sizeRes), &(nodePtr->pages));
                                        fclose(f);
                                        // To avoid divide by 0
                                        if (nodePtr->sizeTotal > 0)
                                                nodePtr->memUtil = (nodePtr->sizeRes/(1.0*nodePtr->sizeTotal))*100;
                                        else
                                                nodePtr->memUtil = 0.0;
                                } else
                                        printf ("Error! %s file not found\n", fname);
 
                                // Process counters
                                procPtr->procTotal ++;
                                if (nodePtr->state == 'R')
                                        procPtr->procRun ++;
                                else if (nodePtr->state == 'S')
                                        procPtr->procSleep ++;
                                else if (nodePtr->state == 'D')
                                        procPtr->procDSleep ++;
                                else if (nodePtr->state == 'Z')
                                        procPtr->procZombie ++;
                                else if (nodePtr->state == 'T')
                                        procPtr->procTrace ++;
                                else if (nodePtr->state == 'W')
                                        procPtr->procPaging ++;
                        }
                }
        } else {
                printf ("Error! Can not open /proc\n");
        }
}

int is_process (char *str)
{
        int i;
        int val = 1;

        for (i = 0;str[i] != '\0' && val;i ++)
            val = isdigit(str[i]);
        return val;
}
