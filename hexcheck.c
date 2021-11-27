#include "stdio.h"
#include "stdlib.h"
#include <stdbool.h>

#define BUFSIZE 2*1024

// some useful ascii codes
const unsigned char ZERO = 48;
const unsigned char LF = 253;
const unsigned char SPACE = 254;
const unsigned char NONHEX = 255;

// returns lower case ascii value of a
// hexadecimal digit a-f turned into A-F
// LF and space left unchanged
// otherwise returns NONHEX indicating
// unacceptable hexadecimal data

unsigned char ishex(unsigned char n) { 
  if (n == 10) return (LF); // allow LF, encode here as 253
  if (n == 32) return (SPACE); // allow space, encode as 254
  if (n < ZERO) return (NONHEX); // reject all others below '0' (ascii 48)
  if (n < 58) return (n-ZERO); // convert digits to numbers 0-9
  if ((n > 96) && (n < 103)) return (n-87); // accept a-f, convert to 10-15
  if ((n > 64) && (n < 71))  return (n-55); // accept A-F, also convert to 10-15
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
  unsigned char buff[BUFSIZE+1];
  int numread, i;

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

  while ((numread = fread(buff,1,BUFSIZE,fin)) > 0) {
    for (i = 0; i < numread; i++) { 
// while ((cin = fgetc(fin)) != EOF) {  
//    uc = ishex((unsigned char)cin);
    uc = ishex((unsigned char)buff[i]);
//    fprintf(stderr,"%c:%u ", buff[i],uc);
    if (uc == NONHEX) // non hex character failure exit
    { 
    	fprintf(stderr, "%s invalid char %c\n", progname, (unsigned char)cin);
      fclose(fout);
      fclose(fin);
      exit(EXIT_FAILURE);
    }
    if ((uc == LF) || (uc == SPACE)) continue; // accept but ignore (delete)

    // pack one byte with two hexes
    if ( firsthex ) { // got full byte, write it out
    	byteout |= uc; // bitwise or, packs two hex chars into a single byte
     	if (fputc(byteout, fout) == EOF) {
      	fprintf(stderr, "error in output\n");
      	fclose(fout);
      	exit(EXIT_FAILURE);
      }
	  firsthex = false;
    } 
    else { // this is first hex, save it
    	byteout = uc << 4;
    	firsthex = true;
    }
   }
  }
  fclose(fin);
  fclose(fout);
  exit(EXIT_SUCCESS);
}
