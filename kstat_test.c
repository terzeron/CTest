
#include <stdio.h>
#include <kstat.h>

int
main(int argc, char *argv[])
{
	kstat_ctl_t *kc;
	kstat_t	 *ksp;

	kc = kstat_open();
	for (ksp = kc->kc_chain; ksp != NULL; ksp = ksp->ks_next) {
		if (ksp->ks_type == KSTAT_TYPE_NAMED) {
			kstat_named_t *p = (kstat_named_t*)malloc(ksp->ks_data_size);
			int i,j;
			if (p != NULL) {
				kstat_read(kc, ksp, p);
				for (i=0; i < ksp->ks_ndata; i++) {
					switch (p[i].data_type) {
						case KSTAT_DATA_CHAR:		
							 printf("name=%s\t\t value=",	p[i].name);
							 for (j=0; j < 16; j++) {
								 printf("%02X", p[i].value.c[j] & 0xFF);
								 if ((j+1)%4 == 0)
									 putchar(' ');
							 }
							 printf("\t'");
							 for (j=0; j < 16; j++) {
								 unsigned char t = p[i].value.c[j];
								 putchar(isalnum(t) ? t : '.');
							 }
							 printf("'\n");
							 break;
						case KSTAT_DATA_LONG:
							 printf("name=%s\t\t value=%ld\n",	p[i].name, p[i].value.l);
							 break;
						case KSTAT_DATA_ULONG:
							 printf("name=%s\t\t value=%lu\n",	p[i].name, p[i].value.ul);
							 break;
						case KSTAT_DATA_LONGLONG:
							 printf("name=%s\t\t value=%lld\n", p[i].name, p[i].value.ll);
							 break;
						case KSTAT_DATA_ULONGLONG:
							 printf("name=%s\t\t value=%llu\n", p[i].name, p[i].value.ull);
							 break;
						default:
							 printf("name=%s\t\t value=???????????????\n");
							 break;
					}
				}
			}
		}
	}
	return 0;
}



