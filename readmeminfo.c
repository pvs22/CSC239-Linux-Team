#include <stdio.h>

main()
{
  FILE* f;
  char buffer[2048];
  if (f = fopen("/proc/meminfo", "r")) {
    fread(buffer, 1, 2048, f);
    printf("meminfo: %s", buffer);
  }
}
