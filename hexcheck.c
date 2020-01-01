#include "stdio.h"
#include "stdlib.h"

unsigned int ishex(unsigned int n) 
{ // returns lower case ascii value of a hexadecimal digit
  // ascii of LF and space unchanged
  // otherwise returns zero indicating unacceptable hexadecimal data
	if ( n < 48 ) return(0); // reject below '0'
	if ( n > 102 ) return(0); // reject above 'f'
	if (( n > 64 ) & ( n < 71 )) return(n+32); // accept A-F but change to lower case
	if (( n > 57 ) & ( n < 97 )) return(0); // reject others between '9' and 'a'
	return(n); // accepting 0-9 and a-f
}

// checks if stdin contains only hexadecimal characters NL,space,0-9,A-F,a-f
// any other character causes it to stop immediately and return failure
// it writes all checked hex characeters to stdout, converting A-F to a-f
int main(int argc, char *argv[])
{
  FILE *fin,*fout;
  char *progname = argv[0], *filein, *fileout; 
  unsigned int c;

  switch (argc)
    {
    case 3:
      fileout = argv[2];
      if ((fout = fopen(fileout,"wb")) == NULL)
	    { fprintf(stderr,"%s: failed to open output file %s\n", progname,fileout);
	      exit(EXIT_FAILURE);
	    }
      filein = argv[1];
      if ((fin = fopen(filein,"rb")) == NULL)
	    { fprintf(stderr,"%s: failed to open input hexfile %s\n", progname,filein);
    	  fclose(fout);
    	  exit(EXIT_FAILURE);
    	}
    break;
    case 2:
      if ((fout = fopen("/dev/stdout","wb")) == NULL)
	    { fprintf(stderr,"%s: failed to open /dev/stdout\n", progname);
	      exit(EXIT_FAILURE);
	    }
      filein = argv[1];
      if ((fin = fopen(filein,"rb")) == NULL)
	    { fprintf(stderr,"%s: failed to open input hexfile %s\n", progname,filein);
    	  fclose(fout); 
          exit(EXIT_FAILURE);
       	}
    break;
    case 1:
      if ((fout = fopen("/dev/stdout","wb")) == NULL)
	    { fprintf(stderr,"%s: failed to open /dev/stdout\n", progname);
	      exit(EXIT_FAILURE);
	    }
      if ((fin = fopen("/dev/stdin","rb")) == NULL)
	    { fprintf(stderr,"%s: failed to open /dev/stdin\n", progname);
	      fclose(fout);
	      exit(EXIT_FAILURE);
	    }
    break;
    default:
      fprintf(stderr,"usage: %s [ hexfile [outfile]]\nomitted files mean stdin/out, e.g. in a pipe: .. | %s hexfile | ..\n",progname,progname);
      exit(EXIT_FAILURE);
    }
      
  while((c = fgetc(fin)) != EOF) 
     {
	  if ( c == 10 ) continue; // do not fail on LF, just ignore it
	  if ( n == 32 ) continue; // same for space
	  if ( (c = ishex(c)) > 0 )
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
