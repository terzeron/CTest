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
    
        (void)printf(" device name      bytes read    bytes written"
               "      read ops      write ops\n");
        kc = kstat_open();
        for (ksp = kc->kc_chain; ksp != NULL; ksp = ksp->ks_next) {
            if (ksp->ks_type == KSTAT_TYPE_IO) {
                kstat_io_t  kio;
        
                kstat_read(kc, ksp, &kio);
                (void)printf("%12s      %10llu       %10llu    %10u     %10u\n", 
                       ksp->ks_name, kio.nread, kio.nwritten, 
                       kio.reads, kio.writes);
            }
        }
        return 0;
    }

