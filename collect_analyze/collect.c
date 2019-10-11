#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <limits.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <db.h>

#include "common.h"
#include "heap.h"
#include "zlib.h"

//#define TEST
//#define VERBOSE


bool_t is_closed[MAX_NUM_LOG_FILES];
int num_closed_files = 0;


void
dump_node (const node_t *node_p)
{
  char time_str[DEFAULT_BUF_SIZE];
  struct tm *tm_p;
  time_t t;

  if (node_p == NULL)
    return;

  t = (time_t) node_p->ts;
  tm_p = localtime (&t);
  strftime (time_str, DEFAULT_BUF_SIZE, "%H:%M:%S", tm_p);
  printf ("Node=(ts=%d'%s', pageid=%d, bcookie=%ld, id_len=%d, file_num=%d)\n",
	  node_p->ts, time_str, node_p->pageid, node_p->bcookie, 
	  node_p->id_len, node_p->file_num);
}


DB *
open_db_file (void)
{
  int ret;
  DB *db_p;
  char file[] = "pageid_cnt_map";
  int flags = DB_CREATE;
  int db_type = DB_HASH;

#ifdef VERBOSE
  printf ("open_db_file()\n");
#endif // VERBOSE

  if ((ret = db_create (&db_p, NULL, 0)) != 0)
    {
      err_print ("can't create DB file, %d: %s\n", 
		 ret, db_strerror (ret));
      return NULL;
    }

  if ((ret = db_p->open (db_p, NULL, file, NULL, db_type, flags, 0644)) != 0)
    {
      err_print ("can't open DB file '%s', %d: %s\n",
		 file, ret, db_strerror (ret));
      return NULL;
    }

#if 0  
  if ((ret = db_p->set_cachesize (db_p, 0, cache_size, 0)) != 0)
    {
      err_print ("can't set cache size for DB, %d: %s\n", 
		 ret, db_strerror (ret));
      return NULL;
    }
#endif // 0

  return db_p;
}


void
close_db_file (DB *db_p)
{
#ifdef VERBOSE
  printf ("close_db_file(db_p=%p)\n", db_p);
#endif // VERBOSE

  db_p->close (db_p, 0);
}


int 
get_yesterday_fields (int *year_p, int *month_p, int *day_p)
{
  time_t t;
  struct tm *tm_p;

#ifdef VERBOSE
  printf ("get_current_date(*year_p=%p, *month_p=%p, day_p=%p)\n",
	  year_p, month_p, day_p);
#endif // VERBOSE

  t = time (NULL) - SECONDS_PER_DAY;
  tm_p = localtime (&t);
  if (tm_p == NULL)
    return -1;
  *year_p = tm_p->tm_year + 1900;
  *month_p = tm_p->tm_mon + 1;
  *day_p = tm_p->tm_mday 
#if 1
    - 1
#endif
    ;

  return 0;
}


int
open_gzipped_log_files (const int year, const int month, const int day, 
			gzFile *gzf_p)
{
  DIR *dir1_p, *dir2_p;
  struct dirent *de1_p, *de2_p;
  char path_fmt1[PATH_MAX] = LOG_DIR1_FMT;
  char dir1_name[PATH_MAX];
  char path_fmt2[PATH_MAX] = LOG_DIR2_FMT;
  char dir2_name[PATH_MAX];
  char path_fmt3[PATH_MAX] = LOG_FILE_FMT;
  char file_name[PATH_MAX];
  int num_gzfiles = 0;

#ifdef VERBOSE
  printf ("open_gzipped_log_files(year=%d, month=%d, day=%d)\n", 
	  year, month, day);
#endif // VERBOSE

  sprintf (dir1_name, path_fmt1, year, month, day);
#ifdef VERBOSE
  printf ("dir1_name: %s\n", dir1_name);
#endif // VERBOSE
  dir1_p = opendir (dir1_name);
  if (dir1_p == NULL) 
    {
      err_print ("can't open directory '%s', %d: %s\n", dir1_name,
		 errno, strerror (errno));
      return -1;
    }

  printf ("Opening compressed files");
  while (dir1_p) 
    {
      if ((de1_p = readdir (dir1_p)) == NULL)
	break;
      if (!strcmp (de1_p->d_name, ".") || !strcmp (de1_p->d_name, ".."))
	continue;
#ifdef VERBOSE
      printf ("de1_p->d_name: %s\n", de1_p->d_name);
#endif // VERBOSE
      
      sprintf (dir2_name, path_fmt2, year, month, day, de1_p->d_name);
      dir2_p = opendir (dir2_name);
      if (dir2_p == NULL)
	{
	  err_print ("can't open directory '%s', %d: %s\n", dir2_name,
		     errno, strerror (errno));
	  return -1;
	}

      while (dir2_p) 
	{
	  printf (".");
	  fflush (stdout);
	  if ((de2_p = readdir (dir2_p)) == NULL) 
	    break;
	  if (!strcmp (de2_p->d_name, ".") || !strcmp (de2_p->d_name, ".."))
	    continue;
	  else if (!strncmp (de2_p->d_name, "lcs_1_sum_", 
			     strlen ("lcs_1_sum_")) 
#if 0
		   &&
		   strstr (de2_p->d_name, "-00")
#endif
		   )
	    
	    {
	      sprintf (file_name, path_fmt3, year, month, day, 
		       de1_p->d_name, de2_p->d_name);
	      gzf_p[num_gzfiles++] = gzopen (file_name, "rb");
	      if (gzf_p == NULL)
		{
		  err_print ("can't open gzipped log file '%s', %d: %s\n",
			     file_name, errno, strerror (errno));
		  return -1;
		}
#if 0
	      printf ("\n");
	      return 1;
#endif
	    }
	}
      closedir (dir2_p);
    }
  closedir (dir1_p);
  printf ("\n");

  return num_gzfiles;
}


void
close_gzipped_log_files (const gzFile *gzf_p, const int num_gzfiles)
{
  int i;

#ifdef VERBOSE
  printf ("close_gzipped_log_files(gzf_p=%p, num_gzfiles=%d)\n", 
	  gzf_p, num_gzfiles);
#endif // VERBOSE

  for (i = 0; i < num_gzfiles; i++) 
    gzclose(gzf_p[i]);
}


int
read_item_from_file (const gzFile *gzf_p, const int gzfile_num, node_t *node_p)
{
  char buf[DEFAULT_BUF_SIZE];

#ifdef VERBOSE
  printf ("read_item_from_file (gzf_p=%p, gzfile_num=%d, node_p=%p)\n",
	  gzf_p, gzfile_num, node_p);
#endif // VERBOSE

  if (gzf_p == NULL)
    {
      err_print ("can't identify file descriptor\n");
      return -1;
    }

  if (gzfile_num < 0) 
    {
      err_print ("can't use invalid file number\n");
      return -1;
    }

  if (node_p == NULL)
    {
      err_print ("can't identify node\n");
      return -1;
    }

  if (gzeof (gzf_p[gzfile_num]))
    {
      if (is_closed[gzfile_num] == FALSE)
	{
	  printf ("##### gzfile_num=%d\n", gzfile_num);
	  num_closed_files++;
	  is_closed[gzfile_num] = TRUE;
	}
#ifdef VERBOSE
      printf ("End-of-file of %dth file\n", gzfile_num);
#endif // VERBOSE
      return 0;
    }

  // 4 byte timestamp
  gzread (gzf_p[gzfile_num], buf, sizeof (u_int32_t));
  node_p->ts = *(u_int32_t *) buf;
  // 4 byte pageid
  gzread (gzf_p[gzfile_num], buf, sizeof (u_int32_t));
  node_p->pageid = *(u_int32_t *) buf;
  // 8 byte bcookie
  gzread (gzf_p[gzfile_num], buf, sizeof (u_int64_t));
  node_p->bcookie = *(u_int64_t *) buf;
  // 4 byte length
  gzread (gzf_p[gzfile_num], buf, sizeof (u_int32_t));
  node_p->id_len = *(u_int32_t *) buf;
  if (node_p->id_len > 0)
    // n byte loginid
    gzread (gzf_p[gzfile_num], buf, node_p->id_len);

  return 1;
}


int
process_items (gzFile *gzf_p, int num_gzfiles, DB *db_p)
{
  int i;
  int ret;
  item_t *item_p;
  item_t new_item;
  DBT key;
  DBT data;
  heap_t hp;
  node_t *node_p;
  node_t *new_node_p;
  int num_items = 0;
  time_t t;
  int val;

#ifdef VERBOSE0
  printf ("process_items(gzf_p=%p, num_gzfiles=%d)\n", gzf_p, num_gzfiles);
#endif // VERBOSE

  // heap initialize
  hp = heap_new ();

  // make a heap sorted with timestamp
  printf ("Making a min heap\n");
  for (i = 0; i < num_gzfiles; i++) 
    {
      node_p = (node_t *) malloc (sizeof (node_t));	  
      if (node_p == NULL)
	{
	  err_print ("can't allocate memory for node, %d: %s\n",
		     errno, strerror (errno));
	  return -1;
	}
      ret = read_item_from_file (gzf_p, i, node_p);
      if (ret < 0)
	goto end_of_read_loop;
      else if (ret == 0)
	{
	  if (num_closed_files == num_gzfiles)
	    break;
	  else
	    continue;
	}
      
      node_p->file_num = i;
#ifdef VERBOSE 
      printf ("file_num=%d ", i);
      dump_node (node_p);
#endif // VERBOSE
      
      // insert this data item to priority queue
      if (heap_insert (hp, node_p, node_p->ts) < 0)
	{
	  err_print ("can't insert item to heap, %d: %s\n",
		     errno, strerror (errno));
	  return -1;
	}
    }

  // then select the first one repeatedly
  printf ("Processing all data\n");
  t = time (NULL);
  while (1)
    {
      ret = heap_pop (hp, (void **) &node_p, &val);
      if (ret == 0)
	{
	  printf ("Empty heap, processing ends.\n");
	  break;
	}
      else if (ret < 0)
	{
	  err_print ("can't get item from heap, %d: %s\n",
		     errno, strerror (errno));
	  return -1;
	}
#ifdef VERBOSE
      printf ("From top ");
      dump_node (node_p);
#endif // VERBOSE

      memset (&key, 0, sizeof (DBT));
      memset (&data, 0, sizeof (DBT));
      key.data = &node_p->bcookie;
      key.size = sizeof (u_int64_t);
      
      ret = db_p->get (db_p, NULL, &key, &data, 0);
      if (ret != 0)
	{
	  if (ret != DB_NOTFOUND)
	    {
	      err_print ("can't get value from DB, %d: %s\n",
			 ret, db_strerror (ret));
	      return -1;
	    }
	  // not found
	  //printf ("not found\n");
	  new_item.is_specified_userid = FALSE;
	  new_item.pageid = node_p->pageid;
	  new_item.login_pageid = 0;
	  new_item.logout_pageid = 0;
	}
      else
	{
	  // found
	  //printf ("found\n");
	  item_p = (item_t *) data.data;
	  
	  if (node_p->id_len > 0)
	    new_item.is_specified_userid = TRUE;
	  else
	    new_item.is_specified_userid = FALSE;
	  new_item.pageid = node_p->pageid;
	  new_item.login_pageid = item_p->login_pageid;
	  new_item.logout_pageid = item_p->logout_pageid;
	  if (item_p->is_specified_userid == TRUE)
	    {
	      if (node_p->id_len == 0)
		// logged in --> logged out
		// remember the new pageid
		new_item.logout_pageid = node_p->pageid;
	    }
	  else
	    {
	      if (node_p->id_len > 0)
		// logged out --> logged in
		// remember the old pageid
		new_item.login_pageid = item_p->pageid;
	    }
	}
	      
      memset (&key, 0, sizeof (DBT));
      memset (&data, 0, sizeof (DBT));
      key.data = &node_p->bcookie;
      key.size = sizeof (u_int64_t);
      data.data = &new_item;
      data.size = sizeof (item_t);
      if ((ret = db_p->put (db_p, NULL, &key, &data, 0)) != 0)
	{
	  err_print ("can't put value into DB, %d: %s\n",
		     ret, db_strerror (ret));
	  return -1;
	}
      
#if 1
      {
	time_t temp_ts;
	temp_ts = time (NULL);
	temp_ts = ((time_t) (temp_ts / 86400)) * 86400 - (24 - 9) * 3600;
	if (node_p->ts > temp_ts + 60 * 60 * 2)
	  {
	    printf ("End-of-time range, temp_ts=%d, ts=%d\n", (int) temp_ts, node_p->ts);
	    break;
	  }
      }
#endif

      new_node_p = (node_t *) malloc (sizeof (node_t));
      ret = read_item_from_file (gzf_p, node_p->file_num, new_node_p);
      if (ret < 0)
	goto end_of_read_loop;
      else if (ret == 0)
	continue;

      new_node_p->file_num = node_p->file_num;
#ifdef VERBOSE
      printf ("file_num=%d ", new_node_p->file_num);
      dump_node (new_node_p);
      printf ("insert a node to heap, file_num=%d\n", new_node_p->file_num);
#endif // VERBOSE
      free (node_p);

      heap_insert (hp, new_node_p, new_node_p->ts);

      ++num_items;

      if (num_items == 1)
	printf ("ts=%d\n", new_node_p->ts);

      if (num_items % 250000 == 250000 - 1) 
	{
	  putchar ('.');
	  fflush (stdout);
	}
      if (num_items % 10000000 == 10000000 - 1)
	{
	  printf (" %ld seconds, ts=%d\n", time (NULL) - t, new_node_p->ts); 
	  t = time (NULL);
	}
    } // while (1)

 end_of_read_loop:
  printf ("\n");
  heap_destroy (hp);

  return 0;
}


int 
main (int argc, char *argv[])
{
  DB *db_p;
  gzFile gzfs[MAX_NUM_LOG_FILES];
  int num_gzfiles;
  int year, month, day;

  memset (is_closed, 0, sizeof (is_closed));

  if ((db_p = open_db_file ()) == NULL) 
    {
      err_print ("can't open DB file, %d: %s\n", errno, strerror (errno));
      return -1;
    }

  if (get_yesterday_fields (&year, &month, &day) < 0) 
    {
      err_print ("can't get current time, %d: %s\n", errno, strerror (errno));
      return -1;
    }

  num_gzfiles = open_gzipped_log_files (year, month, day, gzfs);
  if (num_gzfiles < 0) 
    {
      err_print ("can't open log files, %d: %s\n", errno, strerror (errno));
      return -1;
    }

  if (process_items (gzfs, num_gzfiles, db_p) < 0)
    {
      err_print ("can't read log items, %d: %s\n", errno, strerror (errno));
      return -1;
    }

  close_gzipped_log_files (gzfs, num_gzfiles);

  close_db_file (db_p);

  return 0;
}
