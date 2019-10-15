#include <pcap.h>
//#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define PROGRAM_NAME "arpproxy"
#define CONFFILE_NAME "/etc/rc.conf.local"
#define LEASE_INCREMENT 1.5
#define IPADDRLEN 16
#define ETHERADDRLEN 6
#define ETHERSTRLEN 17
#define MAPFILENAMELEN 80
#define KEY ((key_t) 13957) 

// default configuration
#define DEFAULT_TIMEOUT 30
#define DEFAULT_MAP_FILE "map"
#define DEFAULT_GATEWAY "10.0.0.1"
#define DEFAULT_DEVICE "dc0"

// fflag
#define NO_FLUSH 0x0
#define TIMEOUT_FLUSH 0x1
#define COMPULSIVE_FLUSH 0x2

// rflag
#define NO_ALIAS 0x000
#define ADD_ALIAS 0x010
#define DELETE_ALIAS 0x020
#define ADD_ROUTE 0x100
#define DELETE_ROUTE 0x200
#define ALL_RE_REGISTRATION (ADD_ALIAS | ADD_ROUTE)

// match_prefix return value
#define IGNORE_REQUEST 1
#define UPDATE_ROUTE 2
#define UPDATE_ALIAS 3
#define REGISTER_ROUTE 4
#define REGISTER_ALIAS 5

// sort field
#define SRC 1
#define SRC_HA 2
#define PING 3
#define SRC_REG_TIME 4
#define DST 5
#define DST_TYPE 6
#define DST_REG_TIME 7

// sort direction
#define ASC 1
#define DESC 2


// capture.c
int arp_cap_start(void);
int add_alias(const char *src, const unsigned char *src_ha, const char *dst);
int if_status(char *this_ha);
int delete_alias(const char *src, const unsigned char *src_ha, 
		 const char *dst);

// route.c
int add_route(const char *src, const unsigned char *src_ha, const char *dst);
int delete_route(const char *src, const unsigned char *src_ha, 
		 const char *dst);

// map.c
int map_initialize(const char *file);
int map_finalize(void);
int add_lease_info(const char *src, const unsigned char *src_ha, 
		   const char *dst, const int dst_type);
int delete_lease_info(const char *src, const unsigned char *src_ha, 
		      const char *dst, const int dst_type, 
		      const int delete_flag);
int exist_lease_info(const char *src, const unsigned char *src_ha, 
		     const char *dst, const int dst_type);
int flush_lease(const char *src, const unsigned char *src_ha, 
		const int delete_flag, const int dst_type);
int update_lease_info(const char *src, const unsigned char *src_ha, 
		      const char *dst);
int show_lease(const int dst_type);
int determine_action(const char *src, const unsigned char *src_ha,
		     const char *dst);
int determine_action2(const char *src, const unsigned char *src_ha, 
		      const char *dst);

// util.c
int str2hex(const char *str, unsigned char *hex);
char *hex2str(const unsigned char *hex, char *str);
int compare_hex(const unsigned char *hex1, const unsigned char *hex2);
int compar(const void *a, const void *b);

// ping.c
int ping(const char *src);

// ping_test.c
int ping_test(void);

// semaphore.c
int semaphore_initialize(void);
void semaphore_finalize(int sem_id);
int semaphore_lock(int sem_id);
int semaphore_unlock(int sem_id);

struct printer {
    pcap_handler f;
    int type;
};

struct lease_header {
    unsigned int total_lease_count;
    unsigned int alias_count;
    unsigned int route_count;
    time_t start_time;
};

struct lease_info {
#define INVALID 0x0000
#define ALIAS 0x1000
#define ROUTE 0x2000
#define ALL 0x3000
    in_addr_t src;
    unsigned char src_ha[ETHERADDRLEN];
    in_addr_t dst;
    short dst_type;
    time_t src_reg_time;
    time_t dst_reg_time;
    short ping_count;
};

#define DEFAULT_MAP_SIZE (sizeof (struct lease_header) + sizeof (struct lease_info) * 10)





