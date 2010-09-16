#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "readperfstats.h"

int is_process (char *str); // checks if the dir name "*str" correspond to a process
void getWallTime (struct wallTime **wt, unsigned long tv); // convert Jiffies to days/hrs/mins/secs/msecs
void getProcStats (struct procStats *procPtr, int iter); // gets process stats from /proc dir
 
void getAllProcStatcs ()
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


