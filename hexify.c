#include "stdio.h"
#include <unistd.h>

// reads from stdin, writes to stdout
int main (void) {
  unsigned char buff[1024];
  int numread, i;

  while ((numread = read(0, buff, 1024)) > 0) {
    for (i = 0; i < numread; i++) {
      printf("%02x", buff[i]);
    }
  }
 printf("\n");
 return 0;
}
