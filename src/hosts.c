/* libnss_econf.so: NSS module using libeconf
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
   License along with program; if not, see <https://www.gnu.org/licenses/>. */

#include "config.h"

#include <arpa/inet.h>
#include <arpa/nameser.h>

#include "nss-econf.h"

#define ENTNAME         hostent
#define DATABASE	"hosts"
#define NEED_H_ERRNO
# define H_ERRNO_SET(val) (*herrnop = (val))

#define EXTRA_ARGS       , af
#define EXTRA_ARGS_DECL  , int af

#define ENTDATA hostent_data
struct hostent_data
  {
    unsigned char host_addr[16]; /* IPv4 or IPv6 address.  */
    char *h_addr_ptrs[2];       /* Points to that and null terminator.  */
  };

#define TRAILING_LIST_MEMBER            h_aliases
#define TRAILING_LIST_SEPARATOR_P       isspace
#include "files-parse.c"
LINE_PARSER
("#",
 {
   char *addr;

   STRING_FIELD (addr, isspace, 1);

   /* Parse address.  */
   if (af != AF_INET6 && inet_pton (AF_INET, addr, entdata->host_addr) > 0)
     {
       result->h_addrtype = AF_INET;
       result->h_length = INADDRSZ;
     }
   else if (af != AF_INET
            && inet_pton (AF_INET6, addr, entdata->host_addr) > 0)
     {
       result->h_addrtype = AF_INET6;
       result->h_length = IN6ADDRSZ;
     }
   else
     /* Illegal address: ignore line.  */
     return 0;

   /* Store a pointer to the address in the expected form.  */
   entdata->h_addr_ptrs[0] = (char *) entdata->host_addr;
   entdata->h_addr_ptrs[1] = NULL;
   result->h_addr_list = entdata->h_addr_ptrs;

   STRING_FIELD (result->h_name, isspace, 1);
 })


static enum nss_status
internal_getent (struct data_t *data, struct hostent *result,
                 char *buffer, size_t buflen, int *errnop, int *herrnop,
		 int af);

#define DELIM " \t"

#define EXTRA_ARGS_VALUE , AF_INET
#include "econf-XXX.c"
#undef EXTRA_ARGS_VALUE

/* Parsing the database file into `struct hostent' data structures.  */
static enum nss_status
internal_getent (struct data_t *data, struct hostent *result,
                 char *buffer, size_t buflen, int *errnop, int *herrnop,
		 int af)
{
  int saved_errno = errno;      /* Do not clobber errno on success.  */
  int ret;

  if (data->key_file == NULL || data->key_number == 0)
    {
      /* No entry */
      H_ERRNO_SET (HOST_NOT_FOUND);
      return NSS_STATUS_NOTFOUND;
    }

  do {

    if (data->next_key >= data->key_number)
      {
	/* All entries returned */
	H_ERRNO_SET (HOST_NOT_FOUND);
	return NSS_STATUS_NOTFOUND;
      }

    char *val = NULL;
    if (econf_getStringValue (data->key_file, NULL, data->keys[data->next_key], &val))
      return NSS_STATUS_NOTFOUND;

    if ((strlen (data->keys[data->next_key]) + 1 + strlen (val) + 1) > buflen)
      {
	*errnop = ERANGE;
	H_ERRNO_SET (NETDB_INTERNAL);
	return NSS_STATUS_TRYAGAIN;
      }

    struct parser_data *pdata = (void *) buffer;
    char *cp = stpcpy (pdata->linebuffer, data->keys[data->next_key]);
    cp = stpcpy (cp, " ");
    strcpy (cp, val);
    free (val);

    ret = _nss_files_parse_hostent (pdata->linebuffer, result, pdata, buflen, errnop, af);
    if (ret == -1)
      return NSS_STATUS_TRYAGAIN;

    data->next_key++;

  } while (ret != 1);

  errno = saved_errno;

  return NSS_STATUS_SUCCESS;
}

/* We only need to consider IPv4 mapped addresses if the input to the
   gethostbyaddr() function is an IPv6 address.  */
#define EXTRA_ARGS_VALUE , af
DB_LOOKUP (hostbyaddr, ,,,
           {
             if (result->h_length == (int) len
                 && ! memcmp (addr, result->h_addr_list[0], len))
               break;
           }, const void *addr, socklen_t len, int af)
#undef EXTRA_ARGS_VALUE


static enum nss_status
internal_gethostbyname2_r (const char *name, int af, struct hostent *result,
                           char *buffer, size_t buflen, int *errnop,
                           int *h_errnop)
{
  struct data_t local_data = {NULL, NULL, 0, 0};
  enum nss_status status = internal_setent (&local_data, DATABASE, DELIM);

  if (status == NSS_STATUS_SUCCESS)
    {
      while ((status = internal_getent (&local_data, result, buffer,
                                        buflen, errnop, h_errnop, af))
	      == NSS_STATUS_SUCCESS)
	{
	  int naliases = 0;

          if (strcasecmp (name, result->h_name) == 0)
	    break;

	  for (; result->h_aliases[naliases] != NULL; ++naliases)
	    if (! strcasecmp (name, result->h_aliases[naliases]))
	      break;
	}

      internal_endent (&local_data);
    }

  return status;

}

enum nss_status
_nss_econf_gethostbyname2_r (const char *name, int af, struct hostent *host,
			     char *buffer, size_t buflen, int *errnop,
			     int *h_errnop)
{
  if (af != AF_INET && af != AF_INET6)
    {
      *h_errnop = HOST_NOT_FOUND;
      return NSS_STATUS_NOTFOUND;
    }

  return internal_gethostbyname2_r (name, af, host, buffer, buflen, errnop,
                                    h_errnop);
}


enum nss_status
_nss_econf_gethostbyname_r (const char *name, struct hostent *host,
			    char *buffer, size_t buflen, int *errnop,
			    int *h_errnop)
{
  return internal_gethostbyname2_r (name, AF_INET, host, buffer, buflen,
                                    errnop, h_errnop);
}
