/* Common code for file-based databases in nss_files module.
   Copyright (C) 1996-2023 Free Software Foundation, Inc.
   This file is part of the GNU C Library.

   The GNU C Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   The GNU C Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <pthread.h>

/* These symbols are defined by the including source file:

   ENTNAME -- database name of the structure and functions (hostent, pwent).
   STRUCTURE -- struct name, define only if not ENTNAME (passwd, group).
   STRUCT_NAME -- field x_name of the STRUCTURE.
   DATABASE -- string of the database file's name ("hosts", "passwd").
   DELIM -- string of delimeter for libeconf (" \t").

   NEED_H_ERRNO - defined iff an arg `int *herrnop' is used.

   Also see files-parse.c.
*/

#define ENTNAME_r	CONCAT(ENTNAME,_r)

#ifdef NEED_H_ERRNO
# include <netdb.h>
# define H_ERRNO_PROTO	, int *herrnop
# define H_ERRNO_ARG	, herrnop
# define H_ERRNO_ARG_OR_NULL herrnop
# define H_ERRNO_SET(val) (*herrnop = (val))
#else
# define H_ERRNO_PROTO
# define H_ERRNO_ARG
# define H_ERRNO_ARG_OR_NULL NULL
# define H_ERRNO_SET(val) ((void) 0)
#endif

#ifndef EXTRA_ARGS
# define EXTRA_ARGS
# define EXTRA_ARGS_DECL
# define EXTRA_ARGS_VALUE
#endif

static struct data_t global_data = {NULL, NULL, 0, 0};
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

/* Thread-safe, exported version.  */
enum nss_status
CONCAT(_nss_econf_set,ENTNAME) (int stayopen __attribute__((__unused__)))
{
  enum nss_status status;

  pthread_mutex_lock (&lock);

  status = internal_setent (&global_data, DATABASE, DELIM);

  pthread_mutex_unlock (&lock);

  return status;
}

enum nss_status
CONCAT(_nss_econf_end,ENTNAME) (void)
{
  pthread_mutex_lock (&lock);

  internal_endent (&global_data);

  pthread_mutex_unlock (&lock);

  return NSS_STATUS_SUCCESS;
}

#ifdef NEED_GETENT
/* Parsing the database file into `struct STRUCTURE' data structures.  */
static enum nss_status
internal_getent (struct data_t *data, struct STRUCTURE *result,
		 char *buffer, size_t buflen, int *errnop H_ERRNO_PROTO
		 EXTRA_ARGS_DECL)
{
  int saved_errno = errno;	/* Do not clobber errno on success.  */

  if (data->key_file == NULL || data->key_number == 0)
    {
      /* No entry */
      H_ERRNO_SET (HOST_NOT_FOUND);
      return NSS_STATUS_NOTFOUND;
    }

  if (data->next_key >= data->key_number)
    {
      /* All entries returned */
      H_ERRNO_SET (HOST_NOT_FOUND);
      return NSS_STATUS_NOTFOUND;
    }

  size_t keylen = strlen (data->keys[data->next_key]);
  if ((keylen + 1) > buflen)
    {
      *errnop = ERANGE;
      H_ERRNO_SET (NETDB_INTERNAL);
      return NSS_STATUS_TRYAGAIN;
    }

  result->STRUCT_NAME = buffer;
  strcpy (result->STRUCT_NAME, data->keys[data->next_key]);
  buflen = buflen - keylen - 1;
  buffer = &buffer[keylen + 1];

  char *val = NULL;
  if (econf_getStringValue (data->key_file, NULL, result->STRUCT_NAME, &val))
    return NSS_STATUS_NOTFOUND;

  if ((strlen (val) + 1) > buflen)
    {
      *errnop = ERANGE;
      H_ERRNO_SET (NETDB_INTERNAL);
      return NSS_STATUS_TRYAGAIN;
    }

  struct parser_data *pdata = (void *) buffer;
  strcpy (pdata->linebuffer, val);
  free (val);

  if (CONCAT(_nss_files_parse_,ENTNAME) (pdata->linebuffer, result, pdata, buflen, errnop) == -1)
    return NSS_STATUS_TRYAGAIN;

  data->next_key++;

  errno = saved_errno;

  return NSS_STATUS_SUCCESS;
}
#endif

#ifdef NEED_GETKEY
static enum nss_status
internal_getkey (struct data_t *data, const char *key,
		 struct STRUCTURE *result, char *buffer, size_t buflen,
		 int *errnop H_ERRNO_PROTO EXTRA_ARGS_DECL)
{
  int saved_errno = errno;	/* Do not clobber errno on success.  */

  if (data->key_file == NULL)
    {
      /* No entry */
      H_ERRNO_SET (HOST_NOT_FOUND);
      return NSS_STATUS_NOTFOUND;
    }

  size_t keylen = strlen (key);
  if ((keylen + 1) > buflen)
    {
      *errnop = ERANGE;
      H_ERRNO_SET (NETDB_INTERNAL);
      return NSS_STATUS_TRYAGAIN;
    }

  result->STRUCT_NAME = buffer;
  strcpy (result->STRUCT_NAME, key);
  buflen = buflen - keylen - 1;
  buffer = &buffer[keylen + 1];

  char *val = NULL;
  if (econf_getStringValue (data->key_file, NULL, result->STRUCT_NAME, &val))
    return NSS_STATUS_NOTFOUND;

  if ((strlen (val) + 1) > buflen)
    {
      *errnop = ERANGE;
      H_ERRNO_SET (NETDB_INTERNAL);
      return NSS_STATUS_TRYAGAIN;
    }

  struct parser_data *pdata = (void *) buffer;
  strcpy (pdata->linebuffer, val);
  free (val);

  if (CONCAT(_nss_files_parse_,ENTNAME) (pdata->linebuffer, result, pdata, buflen, errnop) == -1)
    return NSS_STATUS_TRYAGAIN;

  data->next_key++;

  errno = saved_errno;

  return NSS_STATUS_SUCCESS;
}
#endif

/* Return the next entry from the database file, doing locking.  */
enum nss_status
CONCAT(_nss_econf_get,ENTNAME_r) (struct STRUCTURE *result, char *buffer,
				  size_t buflen, int *errnop H_ERRNO_PROTO)
{
  enum nss_status status = NSS_STATUS_SUCCESS;

  pthread_mutex_lock (&lock);

  if (global_data.key_file == NULL)
    status = internal_setent (&global_data, DATABASE, DELIM);

  if (status == NSS_STATUS_SUCCESS)
    status = internal_getent (&global_data, result, buffer, buflen,
		              errnop H_ERRNO_ARG EXTRA_ARGS_VALUE);

  pthread_mutex_unlock (&lock);

  return status;
}

/* Macro for defining lookup functions for this file-based database.

   NAME is the name of the lookup; e.g. `hostbyname'.

   DB_CHAR, KEYPATTERN, KEYSIZE are ignored here but used by db-XXX.c
   e.g. `1 + sizeof (id) * 4'.

   PROTO is the potentially empty list of other parameters.

   BREAK_IF_MATCH is a block of code which compares `struct STRUCTURE *result'
   to the lookup key arguments and does `break;' if they match.  */

#define DB_LOOKUP(name, db_char, keysize, keypattern, break_if_match, proto...)\
enum nss_status								      \
_nss_econf_get##name##_r (proto,					      \
			  struct STRUCTURE *result, char *buffer,	      \
			  size_t buflen, int *errnop H_ERRNO_PROTO)	      \
{									      \
  struct data_t local_data = {NULL, NULL, 0, 0};                              \
  enum nss_status status = internal_setent (&local_data, DATABASE, DELIM);    \
                                                                              \
  if (status == NSS_STATUS_SUCCESS)                                           \
    {                                                                         \
      while ((status = internal_getent (&local_data, result, buffer,          \
					buflen, errnop H_ERRNO_ARG EXTRA_ARGS_VALUE)) \
	     == NSS_STATUS_SUCCESS)                                           \
	{ break_if_match }                                                    \
                                                                              \
      internal_endent (&local_data);                                          \
    }									      \
									      \
  return status;							      \
}

#define DB_LOOKUP_KEY(fct_name, key)			                      \
enum nss_status								      \
_nss_econf_get##fct_name##_r (const char *key, 				      \
			      struct STRUCTURE *result, char *buffer,	      \
			      size_t buflen, int *errnop H_ERRNO_PROTO)	      \
{									      \
  struct data_t local_data = {NULL, NULL, 0, 0};                              \
  enum nss_status status = internal_setent_nokeys (&local_data, DATABASE, DELIM);\
                                                                              \
  if (status == NSS_STATUS_SUCCESS)                                           \
    {                                                                         \
      status = internal_getkey (&local_data, key, result, buffer,	      \
				buflen, errnop H_ERRNO_ARG);		      \
                                                                              \
      internal_endent (&local_data);                                          \
    }									      \
									      \
  return status;							      \
}
