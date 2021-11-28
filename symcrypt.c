#include "stdio.h"

// performs symmetric XOR encryption between two files
// takes three file arguments: keyfile, datafile, outfile
// the last one or two can be omitted to use
// stdout and stdin respectively (suitable for use in a pipe)

#define BUFSIZE 1024

int main(int argc, char *argv[]) {

  FILE *fin1, *fin2, *fout;
  char *progname = argv[0], *filein1, *filein2, *fileout;
  unsigned char uc;
  unsigned char bufin[BUFSIZE];
  unsigned char bufkey[BUFSIZE];
  unsigned char bufout[BUFSIZE];
  int numread, keyread, i;

  // parse file arguments
  switch (argc) {
  case 4:
    fileout = argv[3];
    if ((fout = fopen(fileout, "wb")) == NULL) {
      fprintf(stderr, "%s: failed to open output file %s\n", progname, fileout);
      return (1);
    }
    filein2 = argv[2];
    if ((fin2 = fopen(filein2, "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open input file %s\n", progname, filein2);
      fclose(fout);
      return (1);
    }
    filein1 = argv[1];
    if ((fin1 = fopen(filein1, "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open input keyfile %s\n", progname,
              filein1);
      fclose(fout);
      fclose(fin2);
      return (1);
    }
    //  printf("%s %s %s %s running...\n",progname,filein1,filein2,fileout);
    break;
  case 3:
    if ((fout = fopen("/dev/stdout", "wb")) == NULL) {
      fprintf(stderr, "%s: failed to open /dev/stdout\n", progname);
      return (1);
    }
    filein2 = argv[2];
    if ((fin2 = fopen(filein2, "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open input file %s\n", progname, filein2);
      fclose(fout);
      return (1);
    }
    filein1 = argv[1];
    if ((fin1 = fopen(filein1, "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open input keyfile %s\n", progname,
              filein1);
      fclose(fout);
      fclose(fin2);
      return (1);
    }
    break;
  case 2:
    if ((fout = fopen("/dev/stdout", "wb")) == NULL) {
      fprintf(stderr, "%s: failed to open /dev/stdout\n", progname);
      return (1);
    }
    if ((fin2 = fopen("/dev/stdin", "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open /dev/stdin\n", progname);
      fclose(fout);
      return (1);
    }
    filein1 = argv[1];
    if ((fin1 = fopen(filein1, "rb")) == NULL) {
      fprintf(stderr, "%s: failed to open input keyfile %s\n", progname,
              filein1);
      fclose(fout);
      fclose(fin2);
      return (1);
    }
    break;
  default:
    fprintf(stderr,
            "usage: %s keyfile [infile [outfile]]\nomitted files mean "
            "stdin/out, e.g. in a pipe: .. | %s keyfile | ..\n",
            progname, progname);
    return (1);
  }
 
  // read file chunks into bufin and bufkey
  while ((numread = fread(bufin,1,BUFSIZE,fin2)) > 0) {
    if ((keyread = fread(bufkey,1,BUFSIZE,fin1)) == 0) {    
        fprintf(stderr,"%s: keyfile read error\n",progname);
        fclose(fout);
        fclose(fin2);
        fclose(fin1);
        return (1);
      }
    // both bufin and bufkey are now filled
    if (numread != keyread) { // can happen on the last reads
        fprintf(stderr,"%s: keyfile and infile differ in lengths\n",progname);
        fclose(fout);
        fclose(fin2);
        fclose(fin1);
        return (1);
      }
    for (i = 0; i < numread; i++) { 
      bufout[i] = bufin[i] ^ bufkey[i]; // this is where it is all done!
    }
    if ( fwrite(bufout,1,numread,fout) == 0 ) {
      fprintf(stderr,"%s failed to write bufout\n", progname); 
      fclose(fout); 
      fclose(fin2);
      fclose(fin1);
      return (1);
    }     
  }
  return (0);
}
