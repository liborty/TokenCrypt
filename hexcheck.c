#include "stdio.h"
#include "stdlib.h"
#include <stdbool.h>

// some useful ascii codes
const unsigned char ZERO = 48;
const unsigned char SPACE = 32;
const unsigned char LF = 10;
const unsigned char NONHEX = 0;

// returns lower case ascii value of a
// hexadecimal digit a-f turned into A-F
// LF and space left unchanged
// otherwise returns NONHEX indicating
// unacceptable hexadecimal data

unsigned char ishex(unsigned char n) { 
  if (n < ZERO) {
    if (n == LF) return (LF); // allow LF
    if (n == SPACE) return (SPACE); // allow space
    return (NONHEX); // reject all others below the digit '0' (ascii 48)
  }
  if ((n > 96) && (n < 103)) return (n - 32); // accept a-f but change to upper case
  if ((n > 64) && (n < 71))  return (n); // A-F
  if (n < 58) return (n); // 0-9
  return (NONHEX); // reject all others  
}
  
// checks if stdin contains only hexadecimal characters NL,space,0-9,A-F,a-f
// any other character causes it to stop immediately and return failure
// it writes all checked and packed hex characters to stdout

int main(int argc, char *argv[]) {
  FILE *fin, *fout;
  char *progname = argv[0], *filein, *fileout;
  int cin;
  bool firsthex = false; // to pack bytes
  unsigned char uc, byteout;

  switch (argc) {
  case 3:
    fileout = argv[2];
    if ((fout = fopen(fileout, "wb")) == NULL) {
      fprintf(stderr, "%s: failed to open output file %s\n", progname, fileout);
      exit(EXIT_FAILURE);
    }
    filein = argv[1];
    if ((fin = fopen(filein, "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open input hexfile %s\n", progname,
              filein);
      fclose(fout);
      exit(EXIT_FAILURE);
    }
    break;
  case 2:
    if ((fout = fopen("/dev/stdout", "wb")) == NULL) {
      fprintf(stderr, "%s: failed to open /dev/stdout\n", progname);
      exit(EXIT_FAILURE);
    }
    filein = argv[1];
    if ((fin = fopen(filein, "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open input hexfile %s\n", progname,
              filein);
      fclose(fout);
      exit(EXIT_FAILURE);
    }
    break;
  case 1:
    if ((fout = fopen("/dev/stdout", "wb")) == NULL) {
      fprintf(stderr, "%s: failed to open /dev/stdout\n", progname);
      exit(EXIT_FAILURE);
    }
    if ((fin = fopen("/dev/stdin", "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open /dev/stdin\n", progname);
      fclose(fout);
      exit(EXIT_FAILURE);
    }
    break;
  default:
    fprintf(stderr,
            "usage: %s [ hexfile [outfile]]\nomitted files mean stdin/out, "
            "e.g. in a pipe: .. | %s hexfile | ..\n",
            progname, progname);
    exit(EXIT_FAILURE);
  }
  while ((cin = fgetc(fin)) != EOF) {
  
    uc = ishex((unsigned char)cin);
    if (uc == NONHEX) // non hex character failure exit
    { 
    	fprintf(stderr, "%s: quitting, found nonhex character %c\n", progname, (unsigned char)cin);
      fclose(fout);
      fclose(fin);
      exit(EXIT_FAILURE);
    }
    if ((uc == LF) || (uc == SPACE)) continue; // accept but delete
    // turn ascii into binary   
    if ( uc < 58 ) uc -= ZERO;  // turns character zero into numerical zero
    else if ( uc < 71 ) uc -= 55; // turns A into 10
    // pack one byte with two hexes
    if ( firsthex ) { // got full byte, write it out
    	byteout = (byteout << 4) + uc;
     	if (fputc(byteout, fout) == EOF) {
      	fprintf(stderr, "error in output\n");
      	fclose(fout);
      	exit(EXIT_FAILURE);
      }
	  firsthex = false;
    } 
    else { // this is first hex, save it
    	byteout = uc;
    	firsthex = true;
    }
  }
  fclose(fin);
  fclose(fout);
  exit(EXIT_SUCCESS);
}
