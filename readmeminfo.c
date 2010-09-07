#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int str2int (char* str, int index);
int main()
{
        FILE* f;
        char buffer[2048];
        char tempBuff[80];
        int memTotal, memUsed, memFree;
        float util;

        if (f = fopen("/proc/meminfo", "r"))
        {
                while (fgets(buffer, sizeof buffer, f))
                {
                        if (strncmp(buffer,"MemTotal:",9) == 0)
                        {
                                memTotal = str2int(buffer, 9);
                        }
                        else if (strncmp(buffer,"MemFree:",8) == 0)
                        {
                                memFree = str2int(buffer, 8);
                        }
                }

                memUsed = memTotal - memFree;
                util = (float) memUsed/(1.0*memTotal)*100;

                printf ("--- Memory Stats ---\n");
                printf ("Free Memory = %d KB\n", memFree);
                printf ("Used Memory = %d KB\n", memUsed);
                printf ("Total Memory = %d KB\n", memTotal);
                printf ("Memory Utilization = %5.2f%%\n", util);
        }
        else
        {
                fprintf(stderr, "Error : Can not open /proc/meminfo");
        }
}

int str2int (char* str, int index)
{
        int i, j;
        char buf[80];
        int val;

        i = index;
        while (str[i] == ' ')
                i ++;

        j = 0;
        while (isdigit(str[i]))
        {
                buf[j] = str[i];
                j ++;i ++;
        }
        sscanf (buf, "%d", &val);

        return (val);
}
