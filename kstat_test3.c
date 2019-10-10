#include <stdio.h>
#include <kstat.h>
#include <sys/sysinfo.h>
#include <sys/dnlc.h>
#include <sys/var.h>
#include <sys/flock.h>

int main(int argc, char *argv[])
{
    kstat_ctl_t *kc;
    kstat_t     *ksp;
	vminfo_t *vi;

    (void)printf(" device name      bytes read    bytes written"
           "      read ops      write ops\n");
    kc = kstat_open();
	ksp = kstat_lookup(kc, NULL, -1, "vminfo");
	vi = (vminfo_t *) malloc(sizeof (vminfo_t));
	kstat_read(kc, ksp, vi);
	printf("%lld\n", vi->freemem);
	
    return 0;
}

