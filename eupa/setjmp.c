#include <stdio.h>
#include <setjmp.h>

#define TRY do{ jmp_buf ex_buf__; switch( setjmp(ex_buf__) ){ case 0: while(1){
#define CATCH(x) break; case x:
#define FINALLY break; } default:
#define ETRY } }while(0)
#define THROW(x) longjmp(ex_buf__, x)

#define FOO_EXCEPTION (1)
#define BAR_EXCEPTION (2)
#define BAZ_EXCEPTION (3)

int
main(int argc, char** argv)
{
    do { 
		jmp_buf ex_buf__; 
		switch (setjmp(ex_buf__)) { 
		case 0: 
			while (1) {
				{
					printf("In Try Statement\n");
					longjmp(ex_buf__, (2));
					printf("I do not appear\n");
				}
				break; 
			case (1):
			{
				printf("Got Foo!\n");
			}
			break; 
			case (2):
			{
				printf("Got Bar!\n");
			}
			break; 
			case (3):
			{
				printf("Got Baz!\n");
			}
			break; 
			} 
		default:
			{
				printf("...et in arcadia Ego\n");
			}
		} 
	} while(0);
}
