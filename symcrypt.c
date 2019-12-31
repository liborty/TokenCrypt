#include "stdio.h"

// performs symmetric XOR encryption between two files

int main(int argc, char *argv[])
{
  FILE *fin1, *fin2, *fout;
  char *progname = argv[0], *filein1, *filein2,  *fileout; 
  int c1,c2;
  unsigned char uc;

  switch (argc)
    {
    case 4:
      fileout = argv[3];
      if ((fout = fopen(fileout,"wb")) == NULL)
	    { fprintf(stderr,"%s: failed to open output file %s\n", progname,fileout);
	    return(1);
	    }
      filein2 = argv[2];
      if ((fin2 = fopen(filein2,"rb")) == NULL)
	    { fprintf(stderr,"%s: failed to open input file %s\n", progname,filein2);
	      fclose(fout);
	      return(1);
	    }
      filein1 = argv[1];
      if ((fin1 = fopen(filein1,"rb")) == NULL)
	    { fprintf(stderr,"%s: failed to open input keyfile %s\n", progname,filein1);
    	  fclose(fout);
    	  fclose(fin2);
    	  return(1);
    	}
  //  printf("%s %s %s %s running...\n",progname,filein1,filein2,fileout);
      break;
    case 3:
      if ((fout = fopen("/dev/stdout","wb")) == NULL)
	    { fprintf(stderr,"%s: failed to open /dev/stdout\n", progname);
	    return(1);
	    }
      filein2 = argv[2];
      if ((fin2 = fopen(filein2,"rb")) == NULL)
	    { fprintf(stderr,"%s: failed to open input file %s\n", progname,filein2);
	      fclose(fout);
	      return(1);
	    }
      filein1 = argv[1];
      if ((fin1 = fopen(filein1,"rb")) == NULL)
	    { fprintf(stderr,"%s: failed to open input keyfile %s\n", progname,filein1);
    	  fclose(fout);
    	  fclose(fin2);
    	  return(1);
    	}
    break;
    case 2:
      if ((fout = fopen("/dev/stdout","wb")) == NULL)
	    { fprintf(stderr,"%s: failed to open /dev/stdout\n", progname);
	      return(1);
	    }
      if ((fin2 = fopen("/dev/stdin","rb")) == NULL)
	    { fprintf(stderr,"%s: failed to open /dev/stdin\n", progname);
	      fclose(fout);
	      return(1);
	    }
      filein1 = argv[1];
      if ((fin1 = fopen(filein1,"rb")) == NULL)
	    { fprintf(stderr,"%s: failed to open input keyfile %s\n", progname,filein1);
    	  fclose(fout);
    	  fclose(fin2);
    	  return(1);
    	}
    break;
    default:
      fprintf(stderr,"usage: %s keyfile [infile [outfile]]\nomitted files mean stdin/out, e.g. in a pipe: .. | %s keyfile | ..\n",progname,progname);
      return(1);
    }
   
   while ((c2 = fgetc(fin2)) != EOF)
     {
       if ((c1 = fgetc(fin1)) == EOF) 
	     {
	       if (! feof(fin1))
    	     {
    	       fprintf(stderr,"%s: error in reading keyfile\n", progname);
    	       fclose(fout); fclose(fin2); fclose(fin1);
    	       return(1);
	         } 
           if (fseek( fin1, 0, SEEK_SET ))
            {
               fprintf(stderr,"%s: keyfile failed to seek to the beginning\n", progname);
               fclose(fout); fclose(fin2); fclose(fin1);
    	       return(1);
            }
    	   if ((c1 = fgetc(fin1)) == EOF) 
    	     {
    	       fprintf(stderr,"%s: error re-reading keyfile, or it is empty!\n", progname);
    	       fclose(fout); fclose(fin2); fclose(fin1);
    	       return(1);
    	     } 
    	 }
       uc = ((unsigned char)c1)^((unsigned char)c2);
       if (fputc(uc,fout) == EOF) 
    	 {
    	   fprintf(stderr,"%s: error in output\n", progname);
           fclose(fout); fclose(fin2); fclose(fin1);
    	   return(1);
    	 } 
      }
   if (! feof(fin2))
     {
       fprintf(stderr,"%s: error in reading inputfile\n", progname);
       fclose(fout); fclose(fin2); fclose(fin1);
     }
   if (fclose(fin2) == EOF)
     fprintf(stderr,"%s: incorrectly closed inputfile\n", progname);
   if (fclose(fin1) == EOF)
     fprintf(stderr,"%s: incorrectly closed keyfile\n", progname);
   if (fclose(fout) == EOF)
     fprintf(stderr,"%s: incorrectly closed output\n", progname);
   // fprintf(stderr,"%s %s finished\n",progname,filein1);
   return(0);
}
