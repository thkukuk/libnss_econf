/*  libnss_econf.so: NSS module using libeconf
    Copyright (C) 2023 Thorsten Kukuk

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with the GNU C Library; if not, see
   <https://www.gnu.org/licenses/>.  */

#include <string.h>
#include <pthread.h>

#include "nss-econf.h"

#define ENTNAME         rpcent
#define DATABASE	"rpc"

#define TRAILING_LIST_MEMBER            r_aliases
#define TRAILING_LIST_SEPARATOR_P       isspace
#include "files-parse.c"
LINE_PARSER
("#",
 /* STRING_FIELD (result->r_name, isspace, 1); */
 INT_FIELD (result->r_number, isspace, 1, 10,);
 )

static struct data_t global_data = {NULL, NULL, 0, 0};
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static enum nss_status
internal_getent (struct data_t *data, struct rpcent *rpc,
		 char *buffer, size_t buflen, int *errnop)
{
  if (data->key_file == NULL || data->key_number == 0)
    /* No entry */
    return NSS_STATUS_NOTFOUND;

  if (data->next_key >= data->key_number)
    /* All entries returned */
    return NSS_STATUS_NOTFOUND;

  size_t keylen = strlen (data->keys[data->next_key]);
  if ((keylen + 1) > buflen)
    {
      *errnop = ERANGE;
      return NSS_STATUS_TRYAGAIN;
    }

  rpc->r_name = buffer;
  strcpy (rpc->r_name, data->keys[data->next_key]);
  buflen = buflen - keylen - 1;
  buffer = &buffer[keylen + 1];

  char *val = NULL;
  if (econf_getStringValue (data->key_file, NULL, rpc->r_name, &val))
    return NSS_STATUS_NOTFOUND;

  if ((strlen (val) + 1) > buflen)
    {
      *errnop = ERANGE;
      return NSS_STATUS_TRYAGAIN;
    }

  strcpy (buffer, val);
  free (val);

  struct parser_data *pdata = (void *) buffer;
  if (_nss_files_parse_rpcent (buffer, rpc, pdata, buflen, errnop) == -1)
    return NSS_STATUS_TRYAGAIN;

  data->next_key++;

  return NSS_STATUS_SUCCESS;
}


enum nss_status
_nss_econf_setrpcent (int stayopen __attribute__((__unused__)))
{
  enum nss_status status;

  pthread_mutex_lock (&lock);

  status = internal_setent (&global_data, DATABASE, " \t");

  pthread_mutex_unlock (&lock);

  return status;
}


enum nss_status
_nss_econf_endrpcent (void)
{
  pthread_mutex_lock (&lock);

  internal_endent (&global_data);

  pthread_mutex_unlock (&lock);

  return NSS_STATUS_SUCCESS;
}


enum nss_status
_nss_econf_getrpcent_r (struct rpcent *result, char *buffer,
			 size_t buflen, int *errnop)
{
  enum nss_status status = NSS_STATUS_SUCCESS;

  pthread_mutex_lock (&lock);

  if (global_data.key_file == NULL)
    status = internal_setent (&global_data, DATABASE, " \t");

  if (status == NSS_STATUS_SUCCESS)
    status = internal_getent (&global_data, result, buffer, buflen, errnop);

  pthread_mutex_unlock (&lock);

  return status;
}

enum nss_status
_nss_econf_getrpcbyname_r (const char *name, struct rpcent *result,
			   char *buffer, size_t buflen, int *errnop)
{
  struct data_t local_data = {NULL, NULL, 0, 0};
  enum nss_status status = internal_setent (&local_data, DATABASE, " \t");

  if (status == NSS_STATUS_SUCCESS)
    {
      while ((status = internal_getent (&local_data, result, buffer, buflen, errnop)) == NSS_STATUS_SUCCESS)
	{
	  char **ap;
	  if (! strcmp (name, result->r_name))
	    break;
	  for (ap = result->r_aliases; *ap; ++ap)
	    if (! strcmp (name, *ap))
	      break;
	  if (*ap)
	    break;
	}
      internal_endent (&local_data);
    }

  return status;
}

enum nss_status
_nss_econf_getrpcbynumber_r (int number, struct rpcent *result,
			     char *buffer, size_t buflen, int *errnop)
{
  struct data_t local_data = {NULL, NULL, 0, 0};
  enum nss_status status = internal_setent (&local_data, DATABASE, " \t");

  if (status == NSS_STATUS_SUCCESS)
    {
      while ((status = internal_getent (&local_data, result, buffer, buflen, errnop)) == NSS_STATUS_SUCCESS)
	{
	  if (result->r_number == number)
	    break;
	}
      internal_endent (&local_data);
    }
  return status;
}
