#ifndef __BDB_H__
#define __BDB_H__

#include <sys/types.h>
#include <mmdb.h>
#include <db.h>

#define TABLE_NAME_LEN 256


typedef unsigned long long tuple_id_t;

struct _record {
    tuple_id_t id;
    size_t len;
    char value[1];
};

typedef unsigned int msg_flag_t;

typedef struct _tuple_msg {
    msg_flag_t msg_flag;
    size_t msg_size;
    msg_flag_t op_flag; 
    char table_name[TABLE_NAME_LEN];
    struct _record *rec;
} *tuple_msg_t;

// msg_flag_t msg_flag
#define REQUEST_BACKUP 1
#define REQUEST_RECOVERY 2
// msg_flag_t op_flag
#define OP_DML 0x10
#define OP_DDL 0x20
#define OP_INSERT 0x11
#define OP_UPDATE 0x12
#define OP_DELETE 0x13

#define OP_TRANSACTION_START 0x21
#define OP_COMMIT 0x22
#define OP_ROLLBACK 0x23
#define OP_PREPARE 0x24
#define OP_CREATE_TABLE 0x25
#define OP_DROP_TABLE 0x26
#define OP_ALTER_TABLE 0x27
#define OP_CREATE_INDEX 0x28
#define OP_DROP_INDEX 0x29

struct _recovery_msg {
    msg_flag_t msg_flag;
    size_t msg_size;
    msg_flag_t req_flag;
    char table_name[TABLE_NAME_LEN];
    tuple_id_t tuple_id;
    char data[1];
};

#define DEBUG
#ifdef DEBUG
#define err_print(_fmt, _args...) fprintf(stderr, "%s:%d: "_fmt, \
__FILE__, __LINE__, ##_args)
#else // DEBUG
#define err_print(_fmt, _args...)
#endif // DEBUG

#define ACTION_ASYNC_BACKUP 1
#define ACTION_SYNC_BACKUP 2
#define ACTION_RECOVERY 3
#define ACTION_USAGE 4

#define DEFAULT_DB_HOME "data"
#define DEFAULT_DATABASE "access.db"


extern DB_ENV *init_env(char *db_home);
extern DB *init_db(DB_ENV *dbenv, char *db_file);
extern int insert_rec(DB_ENV *dbenv, DB *dbp, DBT *key, DBT *data);
extern int retrieve_rec(DB_ENV *dbenv, DB *dbp, DBT *key, DBT *data);
extern int delete_rec(DB_ENV *dbenv, DB *dbp, DBT *key);

extern tuple_msg_t make_msg_from_log(mmdb_log_t log);
extern int apply_to_bdb(DB_ENV *dbenv, DB *dbp, tuple_msg_t msg);

extern tuple_msg_t get_msg_from_bdb(DB_ENV *dbenv, DB *dbp);
extern int put_iamge_to_mdb(mmdb_t mdb, tuple_msg_t msg);

extern int sig_to_mdb();
extern int wait_for_sig();

extern int async_backup(void);
extern int sync_backup(void);
extern int recovery(void);
extern int usage(void);

#endif // __BDB_H__
