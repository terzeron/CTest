/*
 * Department of Mathematics & Computer Science & Statistics
 * 9 5 3 2 3 - 1 0 0
 * Cho Young-il 
 * mail to : terzeron@indra.snu.ac.kr
 * ---------------------------------------------------------
 * mktitle.c 
 * ---------------------------------------------------------
 * This program makes the title of the lyric file and renames it.
 */


#include <stdio.h>
#include <errno.h>
#include <ctype.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>

#define PROGRAM "mktitle"
#define SPECIAL_CHAR " '.,-()&:"

int is_name_char(int ch);

int main(int argc, char *argv[])
{
	FILE *infile;
	char temp[256], name[256];
	char temppath[256] = "/home/terzeron/lyric/";
	int close_cnt = 0;
	int i = 0, j, arg;
	
	if (argc < 2) {
		printf("Usage : %s filename\n", PROGRAM);
		exit(-1);
	}

	for (arg = 1; arg < argc; arg++) {
		infile = fopen(argv[arg], "r");
		if (infile == NULL) {
			printf("Can't open such file!\n");
			exit(-1);
		}
		//printf("%s\n", argv[arg]);

		/* initialization */
		close_cnt = 0;
		bzero(temp, 256);
		bzero(name, 256);
		fgets(temp, 256, infile);
		
		if (temp[0] == '<' && temp[1] == '<' && temp[2] == ' ') {
			i = 3;
			j = 0;
		} else {
			printf("This file is not a lyric file!\n");
			continue;
			/*
			exit(-1);
			*/
		}

		while (1) {
			if (is_name_char(temp[i])) {
				if (temp[i] == ' ' || temp[i] == '\t')
					name[j] = '_';
				else 
					name[j] = temp[i];
				i++;
				j++;
			} else if (temp[i] == '>') {
				close_cnt++;
				if (close_cnt == 2) {
					name[j-1] = '\0';
					break;
				}
			} else 
				i++;
		}

		if (strcmp(argv[arg], name)) {
			if (open(name, O_CREAT | O_EXCL) < 0) {
				strcat(temppath, name);
				printf("Already exists %s --> %s\n", argv[arg], temppath);
				rename(argv[arg], temppath);
			} else {
				printf("%s --> %s\n", argv[arg], name); 
				rename(argv[arg], name);
			}
		}
		fclose(infile);
		bzero(temppath, 256);
		j = 0;
	}

	return(1);
}

int is_name_char(int ch)
{
	int i = 0;
	char string[] = SPECIAL_CHAR;
	
	if (isalnum(ch))
		return 1;
	
	while (string[i]) {
		if (ch == string[i])
			return 1;
		else
			i++;
	}
	
	return 0;
}

