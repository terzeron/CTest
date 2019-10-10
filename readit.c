/*
 * Department of Mathematics & Computer Science & Statistics
 * 9 5 3 2 3 - 1 0 0
 * Cho Young-il 
 * mail to : terzeron@indra.snu.ac.kr
 * ---------------------------------------------------------
 * readit - readit.c
 * ---------------------------------------------------------
 * This program edit ascii text file to read comfortably.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PROGRAM "readit"
#define LINELENGTH 80

int main(int argc, char *argv[])
{
    FILE *infile, *outfile = stdout;
    char tmpstr[1024];
    int ch, i = 0;
    int std_length = LINELENGTH;
    int linelen = 0;
  
    if (argc < 2) {
        fprintf(stderr, "Usage : %s input_file output_file\n", PROGRAM);
        exit(1);
    }
  
    infile = fopen(argv[1], "r");
  
    if (argc == 3) {
        outfile = fopen(argv[2], "w");
    }
  
    if (infile == NULL || outfile == NULL) {
        fprintf(stderr, "%s : can't open file(s) for read or write.\n", PROGRAM);
        exit(1);
    }
  
  
    while ((ch = fgetc(infile)) != EOF) {
        if (!isspace(ch)) {
            tmpstr[i++] = ch;
        } else if (linelen + i < std_length) {
            tmpstr[i++] = ch;
            fputs(tmpstr, outfile);
            linelen += i;
            i = 0;
        } else {
            fputs(tmpstr, outfile);
            fputc('\n', outfile);
            i = 0;
        }
    }
    return 0;
}
  
  
  
  
