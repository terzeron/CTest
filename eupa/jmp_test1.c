#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>
#include <unistd.h>

#define FILE_ERROR	1
#define MEM_ERROR	2

jmp_buf jmpbuf;


FILE * find_file(const char *fname)
{
	FILE *f;
 
	if (access(fname, F_OK))
		longjmp(jmpbuf, FILE_ERROR);

	f = fopen(fname, "r");
	if (f == NULL)
		longjmp(jmpbuf, FILE_ERROR);

	return f;
}

void read_file(FILE *f, int size)
{
	char *buf;
 
	if (size < 0)
		longjmp(jmpbuf, MEM_ERROR);

	buf = malloc(size);
	if (buf == NULL)
		longjmp(jmpbuf, MEM_ERROR);

	fread(buf, size, 1, f);

	/* do something */
 
	free(buf);
}


int main(int argc, char *argv[])
{
	int res;
	int step;
	FILE *fp;

	/* Usage: ./a.out <filename> <bufsize> */

	step = -1;
	if ((res = setjmp(jmpbuf)) == 0) {
		step = 0;
		fp = find_file(argv[1]);
		step = 1;
		read_file(fp, strtol(argv[2], NULL, 10));
		step = 2;
		fclose(fp);
	} else if (res == FILE_ERROR) {
		printf("file exception: %s\n", argv[1]);
	} else if (res == MEM_ERROR) {
		printf("memory exception: %s\n", argv[2]);
		fclose(fp);
	}

	printf("last step = %d\n", step);
	// 파일을 못 찾으면 0
	// 사이즈가 너무 크면 1
	// 파일도 찾고 사이즈도 적당하면 2
	return res;
}

