#include "stdio.h"
#include "stdlib.h"

unsigned char ishex(unsigned char n) { // returns lower case ascii value of a
                                       // hexadecimal digit A-F turned into a-f
                                       // LF and space left unchanged
                                       // otherwise returns zero indicating
                                       // unacceptable hexadecimal data

  if (n > 102)
    return (0); // reject above 'f'
  if (n < 48) {
    if (n == 10)
      return (10); // allow LF
    if (n == 32)
      return (32); // allow space
    return (0);    // reject all others below the digit '0' (ascii 48)
  }
  if ((n > 64) && (n < 71))
    return (n + 32); // accept A-F but change to lower case
  if ((n > 57) && (n < 97))
    return (0); // reject all others between '9' and 'a'
  return (n);   // accept remaining valid 0-9 and a-f
}

// checks if stdin contains only hexadecimal characters NL,space,0-9,A-F,a-f
// any other character causes it to stop immediately and return failure
// it writes all checked hex characters to stdout, converting A-F to a-f

int main(int argc, char *argv[]) {
  FILE *fin, *fout;
  char *progname = argv[0], *filein, *fileout;
  int cin;
  unsigned char firsthex = 0; // to pack bytes
  unsigned char uc;

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
    if (uc == 0) // non hex character failure exit
    {
      fclose(fout);
      fclose(fin);
      exit(EXIT_FAILURE);
    }
    // pack bytes with two hexes here
    if (fputc(uc, fout) == EOF) {
      fprintf(stderr, "%s: error in output\n", progname);
      fclose(fout);
      fclose(fin);
      exit(EXIT_FAILURE);
    }
  }
  fclose(fin);
  fclose(fout);
  exit(EXIT_SUCCESS);
}
