#include <stdio.h>
#include <db.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"


int
display (void)
{
  DBC *dbc_p;
  DBT key, data;
  int ret, t_ret;
  char file[] = "pageid_cnt_map";
  DB *db_p;
  item_t *item_p;

  if (db_create (&db_p, NULL, 0) != 0)
    {
      err_print ("can't create DB file, %d: %s\n", errno, strerror (errno));
      return -1;
    }

  if (db_p->open (db_p, NULL, file, NULL, DB_HASH, DB_RDONLY, 0644) != 0)
    {
      err_print ("can't open DB file '%s', %d: %s\n",
		 file, errno, strerror (errno));
      return -1;
    }

  memset (&key, 0, sizeof (DBT));
  memset (&data, 0, sizeof (DBT));

  /* Review the database in 5MB chunks. */
#define BUFFER_LENGTH (5 * 1024 * 1024) if ((data.data = malloc(BUFFER_LENGTH)) == NULL) return (errno); data.ulen = BUFFER_LENGTH; data.flags = DB_DBT_USERMEM;

  /* Acquire a cursor for the database. */
  if ((ret = db_p->cursor (db_p, NULL, &dbc_p, 0)) != 0)
    {
      db_p->err (db_p, ret, "DB->cursor");
      free (data.data);
      return (ret);
    }

  /* Walk through the database and print out the key/data pairs. */
  while ((ret = dbc_p->c_get (dbc_p, &key, &data, DB_NEXT)) == 0)
    {
      item_p = (item_t *) data.data;
      if (item_p->login_pageid == 0 && item_p->logout_pageid == 0)
	continue;
      printf ("%lu : %u %u-->%u\n", *(u_int64_t *) key.data, item_p->pageid, 
	      item_p->login_pageid, item_p->logout_pageid);
    }
  if (ret != DB_NOTFOUND)
    {
      db_p->err (db_p, ret, "DBcursor->get");
    }

  if ((t_ret = dbc_p->c_close (dbc_p)) != 0)
    {
      db_p->err (db_p, ret, "DBcursor->close");
      if (ret == 0)
	ret = t_ret;
    }

  free (data.data);

  return (ret);
}


int
main (void)
{
  display ();
  return 0;
}
