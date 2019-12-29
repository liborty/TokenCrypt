#include "stdio.h"
#include "stdlib.h"

int ishex(int n) 
{ // returns lower case ascii value of a hexadecimal digit or zero
	if ( n < 48 ) return(0);
	if ( n > 102 ) return(0);
	if (( n > 64 ) & ( n < 71 )) return(n+32);
	if (( n > 57 ) & ( n < 97 )) return(0);
	return(n);
}

int main(int argc, char *argv[])
{
  FILE *fin,*fout;
  char *progname = argv[0], *filein, *fileout; 
  int c;

  if ((fout = fopen("/dev/stdout","wb")) == NULL)
	    { fprintf(stderr,"%s: failed to open /dev/stdout\n", progname);
	      exit(EXIT_FAILURE);
	    }
  if ((fin = fopen("/dev/stdin","rb")) == NULL)
	    { fprintf(stderr,"%s: failed to open /dev/stdin\n", progname);
	      fclose(fout);
	      exit(EXIT_FAILURE);
	    }
    
  while((c = fgetc(fin)) != EOF) 
     {
		 if ((c = ishex(c)))
		 {
			if (fputc((unsigned char)c,fout) == EOF) 
			{
				fprintf(stderr,"%s: error in output\n", progname);
				fclose(fout); fclose(fin);
				exit(EXIT_FAILURE);
			}
		 }
		 else exit(EXIT_FAILURE);
	 }
  fclose(fin);
  fclose(fout);
  exit(EXIT_SUCCESS);    
}
