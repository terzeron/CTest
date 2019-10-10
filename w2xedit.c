#include<stdio.h>
#include<string.h>

#define PROGRAM "w2xedit"
#define HUMAN 440
#define ORC 456

int main(int argc, char *argv[])
{
  FILE *fp;
  int location;
  int ret;
  unsigned char buffer[3];
  unsigned char buffer1[3];

  if (argc != 3) {
    fprintf(stderr, "Usage : %s savefile [h|o]\n", PROGRAM, PROGRAM);
    exit(-1);
  }

  fp = fopen(argv[1], "r+");
  if (fp == NULL) {
    fprintf(stderr, "%s : can't open file %s\n", PROGRAM, argv[1]);
    exit(-1);
  }

  if (!strcmp(argv[2], "h"))
    location = HUMAN;
  else if (!strcmp(argv[2], "o"))
    location = ORC;
  else {
    fprintf(stderr, "%s : can't distinguish your tribe.\n", PROGRAM);
    exit(-1);
  }

  /* initialization */
  buffer[0] = 0;
  buffer[1] = 0;
  buffer[2] = 5;
  printf("fwrite buffer = %d:%d:%d\n", buffer[0], buffer[1], buffer[2]);

  printf("starting location = %d\n", location);

  /* lumber value edit */
  ret = fseek(fp, location, SEEK_SET);
  if (ret < 0) {
    fprintf(stderr, "%s : can't seek lumber location in %s\n", PROGRAM, argv[2]);
    exit(-1);
  }

  fread(buffer1, 3, 1, fp);
  printf("READ! buffer = %d:%d:%d\n", buffer1[0], buffer1[1], buffer1[2]);
  fseek(fp, -3, SEEK_CUR);


  printf("fwrite return = %d\n", ret);
  fwrite(buffer, 3, 1, fp);
  if (ret < 0) {
    fprintf(stderr, "%s : can't write lumber value\n", PROGRAM);
    exit(-1);
  }
  fseek(fp, -3, SEEK_CUR);

  fread(buffer1, 3, 1, fp);
  printf("READ AGAIN! buffer = %d:%d:%d\n", buffer1[0], buffer1[1], buffer1[2]);
  fseek(fp, -3, SEEK_CUR);

  /* gold value edit */
  ret = fseek(fp, location+64, SEEK_SET);
  if (ret < 0) {
    fprintf(stderr, "%s : can't seek gold location in %s\n", PROGRAM, argv[2]);
    exit(-1);
  }
  ret = fwrite(buffer, 3, 1, fp);
  if (ret < 0) {
    fprintf(stderr, "%s : can't write gold value\n", PROGRAM);
    exit(-1);
  }

  /* oil value edit */
  ret = fseek(fp, location+128, SEEK_SET);
  if (ret < 0) {
    fprintf(stderr, "%s : can't seek oil location in %s\n", PROGRAM, argv[2]);
    exit(-1);
  }
  ret = fwrite(buffer, 3, 1, fp);
  if (ret < 0) {
    fprintf(stderr, "%s : can't write oil value\n", PROGRAM);
    exit(-1);
  }

  fclose(fp);
  return 0;
}




