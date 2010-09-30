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
        long int cpuTime;  // Process CPU utilization  
        float memUtil;  // Process memory utilization
	long int wallTime;// Running time (wall time)	
        char cmd[81];   // Process Command
	uint64_t priority; // Process Priority
	uint64_t userID;  // UserId
        struct procStatNode *nxtProc;
};

typedef struct
{
  uint64_t cpuNum;
  uint64_t system;
  uint64_t user;
  uint64_t idle;
  uint64_t numInterrupts;
} cpu_info;

int read_meminfo(mem_info*);
cpu_info* read_cpuinfo(uint64_t*);
void read_diskioinfo();
ns* read_networkinfo(uint64_t*);
int read_procstats(struct procStats*);
int is_process (char *str); // checks if the dir name "*str" correspond to a process
void getProcStats (struct procStats *procPtr, int iter); // gets process stats from /proc dir

#endif


