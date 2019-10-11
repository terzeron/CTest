#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>


char *first_encode(const char *name, 
                   const char pad,
                   const char sfx,
                   char *dst)
{
    int i = 0;
    int j = 0;
    int k = 0;

    while (name[i] != '\0' && i < 15) {
        k = toupper(name[i++]);
        dst[j++] = 'A' + ((k & 0xF0) >> 4);
        dst[j++] = 'A' + (k & 0x0F);
    }	

    i = 'A' + ((pad & 0xF0) >> 4);
    k = 'A' + (pad & 0x0F);

    while (j < 30) {
        dst[j++] = i;
        dst[j++] = k;
    }
    
    dst[30] = 'A' + ((sfx & 0xF0) >> 4);
    dst[31] = 'A' + (sfx & 0x0F);
    dst[32] = '\0';
    
    return dst;
}


int second_encode(const char *name, 
                  const char pad,
                  const char sfx,
                  const char *scope, 
                  char *dst)
{
    int lenpos;
    int i;
    int j;

    if (first_encode(name, pad, sfx, &dst[1]) == NULL) {
        return -1;
    }
    dst[0] = 0x20;
    lenpos = 33;

    if (*scope != '\0') {
        do {
            for (i = 0, j = lenpos + 1; scope[i] != '.' && scope[i] != '\0'; 
                 i++, j++) {
                dst[j] = toupper(scope[i]);
            }
	    
            dst[lenpos] = (char) i;
            lenpos += i + 1;
            scope += i;
        } while (*(scope++) == '.');

        dst[lenpos] = '\0';
    }
    
    return (lenpos + 1);
}


int second_decode(const char *src, char *dst)
{
    int len;
    int pos;
    int next;
    int srcpos = 1;
    size_t srcmax;

    if (src == NULL) {
        return -1;
    }
    srcmax = strlen(src);

    dst[0] = '\0';

    len = strlen((char *) &src[srcpos]);

    if (len == 0) {
        return 0;
    }
	
    if (len >= srcmax - srcpos) {
        return -1;
    }
    
    memcpy(dst, &src[srcpos + 1], len);

    for (pos = src[srcpos]; (next = dst[pos]) != '\0'; pos += next + 1) {
        dst[pos] = '.';
    }

    return (--len);
}


int first_decode(const char *src, char *name)
{
    int i = 0;
    int suffix;
    int srcpos = 1;
    int srcmax = strlen(src);
    const char *p = NULL;

    if (src == NULL) {
        return -1;
    }
    srcmax = strlen(src);
    p = &src[srcpos];

    if (srcmax - srcpos < 32) {
        name[0] = '\0';
        return -1;
    }

    for (i = 0; i < 32; i++) {
        name[i / 2] = ((((int) p[i] - 'A') << 4) + ((int) p[i+1] - 'A'));
        i++;
    }

    suffix = name[15];
    name[15] = '\0';

    for (i = 14; i >= 0 && name[i] == ' '; i--) {
        name[i] = '\0';
    }
    
    return suffix;
}


#ifdef TEST
int main()
{
    char input[80] = {
        0x20, 0x45, 
        0x43, 0x46, 0x46, 0x45, 0x45, 0x45, 0x45, 0x46, 
        0x4a, 0x43, 0x41, 0x43, 0x41, 0x43, 0x41, 0x43, 
        0x41, 0x43, 0x41, 0x43, 0x41, 0x43, 0x41, 0x43,
        0x41, 0x43, 0x41, 0x43, 0x41, 0x41, 0x44, 0x00
    };
    char output[80];
    int i;

    memset(output, '\0', 80);
    second_decode(input, output);
    printf("sd=%s\n", output);
    for (i = 0; i < 80; i++) {
        printf("0x%02X(%c) ", output[i], output[i]);
    }
    printf("\n");

    memset(output, '\0', 80);
    first_decode(input, output);
    printf("fd=%s\n", output);
    for (i = 0; i < 80; i++) {
        printf("0x%02X(%c) ", output[i], output[i]);
    }
    printf("\n");

    memset(output, '\0', 80);
    first_encode("BUDDY", ' ', '\3', output);
    printf("fe=%s\n", output);
    for (i = 0; i < 80; i++) {
        printf("0x%02X(%c) ", output[i], output[i]);
    }
    printf("\n");

    memset(output, '\0', 80);
    second_encode("BUDDY", ' ', '\0', "", output);
    printf("se=%s\n", output);
    for (i = 0; i < 80; i++) {
        printf("0x%02X(%c) ", output[i], output[i]);
    }
    printf("\n");

    memset(input, '\0', 80);
    first_decode(output, input);
    printf("fd=\n");
    for (i = 0; i < 80; i++) {
        printf("0x%02X(%c) ", input[i], input[i]);
    }
    printf("\n");

    memset(input, '\0', 80);
    second_decode(output, input);
    printf("sd=\n");
    for (i = 0; i < 80; i++) {
        printf("0x%02X(%c) ", input[i], input[i]);
    }
    printf("\n");

    return 0;
}
#endif // TEST



