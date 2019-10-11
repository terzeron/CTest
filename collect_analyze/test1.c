#include <sys/types.h>
#include <stdio.h>
#include <db.h>

#define DATABASE "access.db"

int
main ()
{
  DB *dbp;
  DBT key, data;
  int ret;
  int num;
  u_int64_t count;

  if ((ret = db_create (&dbp, NULL, 0)) != 0)
    {
      fprintf (stderr, "db_create: %s\n", db_strerror (ret));
      exit (1);
    }
  if ((ret =
       dbp->open (dbp, NULL, DATABASE, NULL, DB_HASH, DB_CREATE, 0664)) != 0)
    {
      dbp->err (dbp, ret, "%s", DATABASE);
      goto err;
    }

  memset (&key, 0, sizeof (key));
  memset (&data, 0, sizeof (data));
  num = 100;
  count = 300;
  key.data = &num;
  key.size = sizeof (int);
  data.data = &count;
  data.size = sizeof (u_int64_t);

  if ((ret = dbp->put (dbp, NULL, &key, &data, 0)) == 0)
    printf ("db: %ld: key stored.\n", *(u_int64_t *) key.data);
  else
    {
      dbp->err (dbp, ret, "DB->put");
      goto err;
    }

  memset (&key, 0, sizeof (key));
  memset (&data, 0, sizeof (data));
  num = 200;
  count = 400;
  key.data = &num;
  key.size = sizeof (int);
  data.data = &count;
  data.size = sizeof (u_int64_t);

  if ((ret = dbp->put (dbp, NULL, &key, &data, 0)) == 0)
    printf ("db: %ld: key stored.\n", *(u_int64_t *) key.data);
  else
    {
      dbp->err (dbp, ret, "DB->put");
      goto err;
    }

  memset (&key, 0, sizeof (key));
  memset (&data, 0, sizeof (data));
  num = 100;
  key.data = &num;
  key.size = sizeof (int);
  if ((ret = dbp->get (dbp, NULL, &key, &data, 0)) == 0)
    printf ("db: %d: key retrieved: data was %ld.\n", *(int *) key.data,
	    *(u_int64_t *) data.data);
  else
    {
      dbp->err (dbp, ret, "DB->get");
      goto err;
    }
  
 err:
}
