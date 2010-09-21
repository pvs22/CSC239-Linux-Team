#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#ifndef READPERFSTATS
#define READPERFSTATS

// structure to hold memory (RAM) information.
typedef struct {
    uint64_t free;
    uint64_t used;
    uint64_t total;
    float util;
    uint64_t pgin;
    uint64_t pgout;
    uint64_t swpin;
    uint64_t swpout;
} mem_info;

// structure to hold network information.
typedef struct 
{
  char networkName[256];
  uint64_t bytesRec;
  uint64_t bytesSent;
  uint64_t errs;
  uint64_t colls;
} ns;


// process structure.
struct procStats {
        struct procStatNode *head;
        uint64_t procTotal;  // Total number of processes
        uint64_t procRun;    // Running processes
        uint64_t procSleep;  // Sleeping processes
        uint64_t procDSleep; // Disk Sleeping processes
        uint64_t procZombie; // Zombie processes
        uint64_t procTrace;  // Traced/Stopped Processes 
        uint64_t procPaging; // Paging processes
};

struct procStatNode {
        uint64_t pid;        // PID
        uint64_t sizeTotal;  // Total program size
        uint64_t sizeRes;    // Resident Set size
        uint64_t pages;      // Shared pages
        char state;     // Process Status
        float cpuUtil;  // Process CPU utilization
        float memUtil;  // Process memory utilization
        struct wallTime *wt;// Running time (wall time)
        char cmd[81];   // Process Command
	uint64_t priority;
	uint64_t userID;
        struct procStatNode *nxtProc;
};

struct wallTime {
        short int days;
        short int hours;
        short int mins;
        short int secs;
        short int msecs;
};



typedef struct
{
  long int system;
  long int user;
  long int idle;
  long int numInterrupts;
} cpu_info;

int read_meminfo(mem_info*);
int read_cpuinfo(cpu_info*);
void read_diskioinfo();


int read_networkinfo(ns*, uint64_t*);
int read_procstats();


#endif


