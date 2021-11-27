#include "stdio.h"
#include <unistd.h>

// reads binary from stdin using a small buffer,
// writes hex characters to stdout
int main (void) {
  unsigned char bufin[1024];  
  int numread, i;

  while ((numread = read(0, bufin, 1024)) > 0) {
    for (i = 0; i < numread; i++) {
      printf("%02x", bufin[i]);
    }
  }
 printf("\n");
 return 0;
}
