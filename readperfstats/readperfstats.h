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



int readmeminfo(mem_info*);
void readcpuinfo();
void readdiskioinfo();


void readnetworkinfo();
void readprocessinfo();

#endif


