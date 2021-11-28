#include "stdio.h"
#include <stdbool.h>
#define BUFSIZE 2048

// This utility program fails on the first non-hexadecimal character.
// Otherwise writes all verified hexadecimal data is output as binary,
// two hex chars per byte.

// some useful ascii codes
const unsigned char ZERO = 48;
const unsigned char LF = 253;
const unsigned char SPACE = 254;
const unsigned char NONHEX = 255;

// returns numerical values (0-15) of hexadecimal characters 0-9,a-f,A-F
// LF and SPACE are encoded as above, otherwise returns NONHEX, 
// indicating non-hexadecimal data
unsigned char ishex(unsigned char n) { 
  if (n == 10) return (LF); // LF, recode as 253
  if (n == 32) return (SPACE); // SPACE, recode as 254
  if (n < ZERO) return (NONHEX); // reject all others below '0' (ascii 48)
  if (n < 58) return (n-ZERO); // convert digits to numbers 0-9
  if ((n > 96) && (n < 103)) return (n-87); // a-f, convert to 10-15
  if ((n > 64) && (n < 71))  return (n-55); // A-F, also convert to 10-15
  return (NONHEX); // reject all others  
}

int main(int argc, char *argv[]) {
  FILE *fin, *fout;
  char *progname = argv[0], *filein, *fileout;
  bool firsthex = true; // to pack bytes
  unsigned char uc, byteout;
  unsigned char bufin[BUFSIZE];
  unsigned char bufout[BUFSIZE/2];
  unsigned char *bufoutadr;
  int numread, i;

  // parse file arguments
  switch (argc) {
  case 3:
    fileout = argv[2];
    if ((fout = fopen(fileout, "wb")) == NULL) {
      fprintf(stderr, "%s: failed to open output file %s\n", progname, fileout);
      return(1);
    }
    filein = argv[1];
    if ((fin = fopen(filein, "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open input hexfile %s\n", progname,
              filein);
      fclose(fout);
      return(1);
    }
    break;
  case 2:
    if ((fout = fopen("/dev/stdout", "wb")) == NULL) {
      fprintf(stderr, "%s: failed to open /dev/stdout\n", progname);
      return(1);
    }
    filein = argv[1];
    if ((fin = fopen(filein, "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open input hexfile %s\n", progname,
              filein);
      fclose(fout);
      return(1);
    }
    break;
  case 1:
    if ((fout = fopen("/dev/stdout", "wb")) == NULL) {
      fprintf(stderr, "%s: failed to open /dev/stdout\n", progname);
      return(1);
    }
    if ((fin = fopen("/dev/stdin", "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open /dev/stdin\n", progname);
      fclose(fout);
      return(1);
    }
    break;
  default:
    fprintf(stderr,
            "usage: %s [ hexfile [outfile]]\nomitted files mean stdin/out, "
            "e.g. in a pipe: .. | %s hexfile | ..\n",
            progname, progname);
    return(1);
  }

  // read file chunks into bufin 
  while ((numread = fread(bufin,1,BUFSIZE,fin)) > 0) {
    if (numread % 2 == 1) { // this may arise only for the last chunk
      fprintf(stderr, "%s: input is of odd length, quitting\n", progname);
      fclose(fin);
      fclose(fout);
      return(1);
    }
    bufoutadr = bufout; // reset the output buffer pointer to the beginning
    for (i = 0; i < numread; i++) { 
      uc = ishex(bufin[i]); // get hex values
      if (uc == NONHEX) { // non hex, failure return
      	fprintf(stderr, "%s invalid char %c\n", progname, bufin[i]);
        fclose(fin);
        fclose(fout); 
        return(1);
      }
      if ((uc == LF) || (uc == SPACE)) continue; // ignore (delete) these
      // pack one byte with two hexes
      if (firsthex) { // this is first hex, shift it
    	  *bufoutadr = (unsigned char)(uc << 4);
    	  firsthex = false;
      } else {
    	  *bufoutadr |= uc; // bitwise or, packs two hex chars into a single byte
	      firsthex = true;
        bufoutadr += 1;
      }      
    }
    // Output buffering has the added benefit here that on failing above 
    // on first non-hex character, the output is not even written out.
    // Whereas writing one char at a time would have uselessly written lots
    if ( fwrite(bufout,1,numread/2,fout) == 0 ) {
      fprintf(stderr,"%s failed to write bufout\n", progname);
      fclose(fin);
      fclose(fout); 
      return(1);
    }    
  }
  fclose(fin);
  fclose(fout);
  return(0);
}
