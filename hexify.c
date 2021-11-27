#include "stdio.h"
#include <unistd.h>
#define BUFSIZE 1024
// reads binary from stdin using a small buffer,
// writes hex characters to stdout
int main (void) {
  unsigned char bufin[BUFSIZE];
  char bufout[2*BUFSIZE]; 
  char *bufoutadr;   
  int numread, i;
  while ((numread = read(0,bufin,BUFSIZE)) > 0) {
    bufoutadr = bufout; // reset to the beginning
    for (i = 0; i < numread; i++) {
      sprintf(bufoutadr,"%02x", bufin[i]);
      bufoutadr += 2;
    }
    write(1,bufout,numread*2);
  }
 return 0;
}
