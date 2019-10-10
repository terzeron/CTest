#include <stdio.h>
#include <setjmp.h>

#define TRY do{ jmp_buf ex_buf__; if( !setjmp(ex_buf__) ){
#define CATCH } else {
#define ETRY } }while(0)
#define THROW longjmp(ex_buf__, 1)

int
main(int argc, char** argv)
{
   TRY
   {
      printf("In Try Statement\n");
      THROW;
      printf("I do not appear\n");
   }
   CATCH
   {
      printf("Got Exception!\n");
   }
   ETRY;

   return 0;
}

/*
 Code Rewrite!
 Ignore do...while 

 do { 
     jmp_buf ex_buf__;
	 if (!setjmp(ex_buf__)) {
         printf("In Try Statement\n");
         longjmp(ex_buf__, 1);
         printf("I Do not appear\n");
     } else {
         printf("Got Exception!\n");
     }
 } while (0);

 */
