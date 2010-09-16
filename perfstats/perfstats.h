#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifndef READPERFSTATS
#define READPERFSTATS

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


// process structure.
struct procStats {
        struct procStatNode *head;
        int procTotal;  // Total number of processes
        int procRun;    // Running processes
        int procSleep;  // Sleeping processes
        int procDSleep; // Disk Sleeping processes
        int procZombie; // Zombie processes
        int procTrace;  // Traced/Stopped Processes 
        int procPaging; // Paging processes
};

struct procStatNode {
        int pid;        // PID
        int sizeTotal;  // Total program size
        int sizeRes;    // Resident Set size
        int pages;      // Shared pages
        char state;     // Process Status
        float cpuUtil;  // Process CPU utilization
        float memUtil;  // Process memory utilization
        struct wallTime *wt;// Running time (wall time)
        char cmd[81];   // Process Command
	int priority;
	int userID;
        struct procStatNode *nxtProc;
};

struct wallTime {
        short int days;
        short int hours;
        short int mins;
        short int secs;
        short int msecs;
};



int readmeminfo(mem_info*);
void readcpuinfo();
void readdiskioinfo();


void readnetworkinfo();
void readprocstats();


#endif


