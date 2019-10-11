#define SECONDS_PER_DAY (60 * 60 * 24)
#define MAX_NUM_LOG_FILES 200
#define MAX_ID_LEN 24
#define LOG_DIR1_FMT "/maple/data/proc/logs/%04d/%02d/%02d/split"
#define LOG_DIR2_FMT "/maple/data/proc/logs/%04d/%02d/%02d/split/%s"
#define LOG_FILE_FMT "/maple/data/proc/logs/%04d/%02d/%02d/split/%s/%s"
#define DEFAULT_BUF_SIZE 1024


typedef enum { FALSE = 0, TRUE = 1 } bool_t;

typedef struct
{
  u_int32_t ts;
  u_int32_t pageid;
  u_int64_t bcookie;
  u_int32_t id_len;
  u_int32_t file_num;
  char loginid[MAX_ID_LEN];
} node_t;

typedef struct
{
  bool_t is_specified_userid;
  int pageid;
  // result
  int login_pageid;
  int logout_pageid;
} item_t;

#ifdef __USE_ISOC99
//#define err_print(_fmt, ...) fprintf (stderr, _fmt, __VA_ARGS__)
#define err_print(...) fprintf(stderr, __VA_ARGS__)
#else
#define err_print(_fmt, _args...) fprintf (stderr, _fmt, ##_args)
#endif
