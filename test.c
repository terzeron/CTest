#include <stdio.h>
int main(void)
{
  char temp[1024] = { 0xC1,0xB6,0xB1,0xB3, 0x20, 0xC8,0xA8,0x20,0xC6,0xE4,0xC0,0xCC,0xC1,0xF6,0xB0,0xA1,0x20,0xBE,0xC6,0xC1,0xD6,0x20,0xB1,0xF2,0xB1,0xDD,0xC7,0xD5,0xB4,0xCF,0xB4,0xD9,0x0D,0x0A,0x0D,0x0A,0xC1,0xC1,0xBD,0xC0,0xB4,0xCF,0xB4,0xD9,0x0D,0x0A,0x0D,0x0A,0xC0,0xBD,0x20,0xB1,0xD7,0xB7,0xB1,0xB5,0xA5,0x20,0x20,0xC7,0xC1,0xB7,0xCE,0xB1,0xD7,0xB7,0xA5,0xC0,0xC7,0x20,0x0D,0x0A,0xBC,0xD2,0xBD,0xBA,0xB0,0xA1,0x20,0xC0,0xD6,0xBE,0xEE,0xBE,0xDF,0x20,0xC7,0xCF,0xB4,0xC2,0xB5,0xA5, 0x20, 0xBC,0xD2,0xBD,0xBA,0x20,0xC0,0xD6,0xB1,0xBA,0xBF,0xE4,0x0D,0x0A,0x0D,0x0A,'G', 'r', 'e', 'a', 't', 0xC7,0xD5,0xB4,0xCF,0xB4,0xD9,0x0D,0x0A } ;
  printf("%s", temp);

}