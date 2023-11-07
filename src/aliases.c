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

#include "config.h"

#include <ctype.h>
#include <errno.h>
#include <string.h>
#include <pthread.h>

#include "nss-econf.h"

#define DATABASE "aliases"

static int
_nss_parse_aliasent (const char *name, char *alias,
		     struct aliasent *result,
		     char *buffer, size_t buflen, int *errnop)
{
  char *first_unused = buffer + strlen (alias) + 1;
  size_t room_left =
    buflen - (buflen % __alignof__ (char *)) - strlen (alias) - 2;
  char *line;

  result->alias_members_len = 0;

  if (strlen (name) >= room_left)
    {
      /* The line is too long for our buffer.  */
    no_more_room:
      *errnop = ERANGE;
      return -1;
    }

  *first_unused = '\0';
  first_unused++;
  strcpy (first_unused, name);
  result->alias_name = first_unused;
  first_unused += strlen (name) + 1;

  /* Adjust the pointer so it is aligned for
     storing pointers.  */
  first_unused += __alignof__ (char *) - 1;
  first_unused -= ((first_unused - (char *) 0) % __alignof__ (char *));
  result->alias_members = (char **) first_unused;

  /* Terminate the line for any case.  */
  char *cp = strpbrk (alias, "#\n");
  if (cp != NULL)
    *cp = '\0';

  line = alias;

  while (*line != '\0')
    {
      /* Skip leading blanks.  */
      while (isspace (*line))
        line++;

      if (*line == '\0')
        break;

      if (room_left < sizeof (char *))
          goto no_more_room;
      room_left -= sizeof (char *);
      result->alias_members[result->alias_members_len] = line;

      while (*line != '\0' && *line != ',')
        line++;

      if (line != result->alias_members[result->alias_members_len])
        {
          *line = '\0';
          line++;
          result->alias_members_len++;
        }
    }
  return result->alias_members_len == 0 ? 0 : 1;
}

static struct data_t global_data = {NULL, NULL, 0, 0};
static pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

static enum nss_status
internal_getent (struct data_t *data, struct aliasent *alias,
		 char *buffer, size_t buflen, int *errnop)
{
  if (data->key_file == NULL || data->key_number == 0)
    /* No entry */
    return NSS_STATUS_NOTFOUND;

  if (data->next_key >= data->key_number)
    /* All entries returned */
    return NSS_STATUS_NOTFOUND;

  char *val = NULL;
  if (econf_getStringValue (data->key_file, NULL,
			    data->keys[data->next_key], &val))
    return NSS_STATUS_NOTFOUND;

  if ((strlen (val) + 1) > buflen)
    {
      *errnop = ERANGE;
      free (val);
      return NSS_STATUS_TRYAGAIN;
    }

  strcpy (buffer, val);
  free (val);

  if (_nss_parse_aliasent (data->keys[data->next_key], buffer, alias,
			   buffer, buflen, errnop) == -1)
    return NSS_STATUS_TRYAGAIN;

  data->next_key++;

  return NSS_STATUS_SUCCESS;
}


enum nss_status
_nss_econf_setaliasent (int stayopen __attribute__((__unused__)))
{
  enum nss_status status;

  pthread_mutex_lock (&lock);

  status = internal_setent (&global_data, DATABASE, ":");

  pthread_mutex_unlock (&lock);

  return status;
}


enum nss_status
_nss_econf_endaliasent (void)
{
  pthread_mutex_lock (&lock);

  internal_endent (&global_data);

  pthread_mutex_unlock (&lock);

  return NSS_STATUS_SUCCESS;
}


enum nss_status
_nss_econf_getaliasent_r (struct aliasent *result, char *buffer,
			 size_t buflen, int *errnop)
{
  enum nss_status status = NSS_STATUS_SUCCESS;

  pthread_mutex_lock (&lock);

  if (global_data.key_file == NULL)
    status = internal_setent (&global_data, DATABASE, ":");

  if (status == NSS_STATUS_SUCCESS)
    status = internal_getent (&global_data, result, buffer, buflen, errnop);

  pthread_mutex_unlock (&lock);

  return status;
}

static enum nss_status
internal_getbyname (struct data_t *data, const char *key,
		    struct aliasent *alias,
		    char *buffer, size_t buflen, int *errnop)
{
  if (data->key_file == NULL)
    /* No entry */
    return NSS_STATUS_NOTFOUND;

  char *val = NULL;
  if (econf_getStringValue (data->key_file, NULL, key, &val))
    return NSS_STATUS_NOTFOUND;

  if ((strlen (val) + 1) > buflen)
    {
      *errnop = ERANGE;
      free (val);
      return NSS_STATUS_TRYAGAIN;
    }
  strcpy (buffer, val);
  free (val);

  if (_nss_parse_aliasent (key, buffer, alias, buffer, buflen, errnop) == -1)
    return NSS_STATUS_TRYAGAIN;

  data->next_key++;

  return NSS_STATUS_SUCCESS;
}


enum nss_status
_nss_econf_getaliasbyname_r (const char *name, struct aliasent *result,
			   char *buffer, size_t buflen, int *errnop)
{
  struct data_t local_data = {NULL, NULL, 0, 0};
  enum nss_status status =
    internal_setent_nokeys (&local_data, DATABASE, ":");

  if (local_data.key_file == NULL)
    return NSS_STATUS_NOTFOUND;

  if (status == NSS_STATUS_SUCCESS)
    status = internal_getbyname (&local_data, name, result,
				 buffer, buflen, errnop);

  internal_endent (&local_data);

  return status;
}
