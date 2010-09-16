#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <dirent.h>
#include "perfstats.h"

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



int readmeminfo(mem_info* meminfo)
{
        FILE* f;

        if (f = fopen("/proc/meminfo", "r")) {
	  if (fscanf(f, "MemTotal: %d kB MemFree: %d kb", &(meminfo->total), &(meminfo->free)) == 0) {
	    fclose(f);
	    fprintf(stderr, "Error : Can not read MemTotal and MemFree from /proc/meminfo\n");
	    return -1;
	  } else {  
	    
	    meminfo->used = meminfo->total - meminfo->free;
	    meminfo->util = (float) meminfo->used/(1.0*meminfo->total)*100;	    
	    fclose(f);
	    }
	} else {
	  fprintf(stderr, "Error : Can not open /proc/meminfo");
	  return -1;
	}
	readvmstats(meminfo);
	return 0;
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


int is_process (char *str); // checks if the dir name "*str" correspond to a process
void getWallTime (struct wallTime **wt, unsigned long tv); // convert Jiffies to days/hrs/mins/secs/msecs
void getProcStats (struct procStats *procPtr, int iter); // gets process stats from /proc dir
 
void readprocstats ()
{
        struct procStats p;
        struct procStatNode *tmp;

        getProcStats (&p, 0);

/*--------- Optional code to check if above function call worked --------*/

        printf ("---Process State Stats---\n");
        printf ("Total         : %d\n", p.procTotal);
        printf ("Running       : %d\n", p.procRun);
        printf ("Sleeping      : %d\n", p.procSleep);
        printf ("Disk Sleeping : %d\n", p.procDSleep);
        printf ("Zombie        : %d\n", p.procZombie);
        printf ("Trace/Stopped : %d\n", p.procTrace);
        printf ("Paging        : %d\n", p.procPaging);
        printf ("---------------------------------------\n");


        while (p.head)
        {
                printf ("PID               : %d\n", p.head->pid);
		printf ("Priority          : %d\n", p.head->priority);
                printf ("User ID           : %d\n", p.head->userID);
                printf ("Total Program Size: %d\n", p.head->sizeTotal);
                printf ("Resident Set  Size: %d\n", p.head->sizeRes);
                printf ("Shared Pages      : %d\n", p.head->pages);
                printf ("State             : %c\n", p.head->state);
                printf ("CPU               : %f\n", p.head->cpuUtil);
                printf ("MEM               : %f\n", p.head->memUtil);
                printf ("%d Day %d Hr %d Min %d Sec %d mSec\n", p.head->wt->days, p.head->wt->hours, p.head->wt->mins, p.head->wt->secs, p.head->wt->msecs);
                printf ("CMD               : %s\n", p.head->cmd);
                printf ("---------------------------------------\n");

                tmp = p.head;
                p.head = p.head->nxtProc;
                free(tmp->wt);
                free(tmp);
        }
        printf ("\nDone...\n");
}

int str2int (char* str, int index)
{
        int i, j;
        char buf1[50];
        int val;

        i = index+1;
        while (str[i] == ' ')
                i ++;

//printf("\nii %d\n",i);
        j = 0;
        while (isdigit(str[i])&& str[i]!=' ')
        {
//		printf("\nstr[i] %c******************\n",str[i]);
                buf1[j] = str[i];
                j ++;i ++;
        }
        sscanf (buf1, "%d", &val);
//printf("\nval %d buff %s--------------------\n",val,buf1);

        return (val);
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
	char buffer[512];

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
                                        fscanf (f, "%d %s %c %d %d %d %d %d %u %lu %lu %lu %lu %lu %lu %lu %ld %ld %ld %ld %llu", &(nodePtr->pid), &(nodePtr->cmd), &(nodePtr->state), &ppid, &pgrp, &session, &tty_nr, &tpgid, &flags, &minflt, &majflt, &cmajflt, &utime, &stime, &cutime, &cstime, &(nodePtr->priority), &nice, &num_thr, &itrealv, &startTime);
                                        fclose(f);

                                        // Calculate CPU utilization here
                                        nodePtr->cpuUtil = (utime + stime)/100.0;
                                        // Assumption startTime/100 = wall time in secs
                                        getWallTime(&(nodePtr->wt), startTime);
                                } else
                                        printf ("Error! %s file not found\n", fname);

                                sprintf (fname, "/proc/%s/status", ent->d_name);
                                if (f = fopen(fname, "r"))
				{
					while (fgets(buffer, sizeof buffer, f))
			                {
			                        if (strncmp(buffer,"Uid:",4) == 0)
			                        {
                                        		//printf ("inside %s  buf %s\n", fname,buffer);
			                                (nodePtr->userID) = str2int(buffer, 4);
			                        }
					}
                                        fclose(f);
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

void getWallTime (struct wallTime **wt, unsigned long tv)
{
        *wt = (struct wallTime *) malloc(sizeof(struct wallTime));
        // 24hrs*60mins*60secs*100 jiffies
        (*wt)->days = tv/8640000;
        tv = tv % 8640000;

        // 60mins*60secs*100 jiffies
        (*wt)->hours = tv/360000;
        tv = tv % 360000;

        // 60secs*100 jiffies
        (*wt)->mins = tv/6000;
        tv = tv % 6000;

        // 100 jiffies
        (*wt)->secs = tv/100;
        tv = tv % 100;

        // 10msecs
        (*wt)->msecs = tv*10;
}


