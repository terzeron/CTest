#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <gdbm.h>

/*
  #define DBM_FILENAME "/usr/local/apache/share/cgi-bin/mp3_cnt"
  */
#define DBM_FILENAME "./mp3_cnt"
#define DBM_BLKSIZ 1024
#define DBM_MODE 0644

int main(int argc, char *argv[])
{
  GDBM_FILE dbf;
  char command[256];
  char mp3name[256];
  datum key, content;
  int count;
  int ret;

  if (argc != 3) {
    printf("Usage: count.pl update|show mp3name\n");
    exit(-1);
  } else {
    if (strlen(argv[1]) > 100 || strlen(argv[2]) > 100) {
      printf("Too long command or mp3name");
      exit(-1);
    }
    strcpy(command, argv[1]);
    strcpy(mp3name, argv[2]);
  }
  
  dbf = gdbm_open(DBM_FILENAME, DBM_BLKSIZ, GDBM_WRCREAT, DBM_MODE, (void *) NULL);
  if (!dbf) {
    printf("Can't open dbm file\n");
    return -1;
  }
  
  if (!strcmp(command, "update")) {
    key.dsize = strlen(mp3name);
    key.dptr = mp3name;
    if (gdbm_exists(dbf, key)) {
      content = gdbm_fetch(dbf, key);
      if (content.dptr == NULL) {
	printf("Can't fetch a datum\n");
	exit(-1);
      }
      sscanf(content.dptr, "%4d", &count);
      count++;
      sprintf(content.dptr, "%4d", count);
    } else {
      content.dsize = sizeof(int);
      content.dptr = (char *) malloc(sizeof(int));
      if (content.dptr == NULL) {
	printf("Can't allocate memory\n");
	exit(-1);
      }
      sprintf(content.dptr, "%4d", 1);
    }
    ret = gdbm_store(dbf, key, content, GDBM_REPLACE);
    if (ret < 0) {
      printf("Can't store a datum\n");
      exit(-1);
    }
    printf("%d", count);
  } else if (!strcmp(command, "show")) {
    key.dsize = strlen(mp3name);
    key.dptr = mp3name;
    if (gdbm_exists(dbf, key)) {
      content = gdbm_fetch(dbf, key);
      if (content.dptr == NULL) {
	printf("Can't fetch a datum\n");
	exit(-1);
      }
      sscanf(content.dptr, "%4d", &count);
      printf("%d", count);
    } else {
      printf("Can't fetch a datum\n");
      exit(-1);
    }
  } else {
    printf("Invalid command\n");
    exit(-1);
  }
  
  
  return 0;
}



