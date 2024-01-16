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

#include <string.h>
#include <pthread.h>

#include "nss-econf.h"

#define ENTNAME         servent
#define DATABASE	"services"

#define TRAILING_LIST_MEMBER            s_aliases
#define TRAILING_LIST_SEPARATOR_P       isspace
#include "files-parse.c"
#define ISSLASH(c) ((c) == '/')
LINE_PARSER
("#",
 STRING_FIELD (result->s_name, isspace, 1);
 INT_FIELD (result->s_port, ISSLASH, 10, 0, htons);
 STRING_FIELD (result->s_proto, isspace, 1);
 )


static enum nss_status
internal_getent (struct data_t *data, struct servent *serv,
		 char *buffer, size_t buflen, int *errnop)
{
  struct parser_data *pdata = (void *) buffer;

  if (data->key_file == NULL || data->key_number == 0)
    /* No entry */
    return NSS_STATUS_NOTFOUND;

  if (data->next_key >= data->key_number)
    /* All entries returned */
    return NSS_STATUS_NOTFOUND;

  if ((strlen (data->keys[data->next_key]) + 1) > buflen)
    {
      *errnop = ERANGE;
      return NSS_STATUS_TRYAGAIN;
    }
  strcpy (buffer, data->keys[data->next_key]);

  if (_nss_files_parse_servent (buffer, serv, pdata, buflen, errnop) == -1)
    return NSS_STATUS_TRYAGAIN;

  data->next_key++;

  return NSS_STATUS_SUCCESS;
}

#define STRUCT_NAME s_name
#define DELIM ""

#include "econf-XXX.c"

DB_LOOKUP (servbyname,
           {
             /* Must match both protocol (if specified) and name.  */
             if (proto != NULL && strcmp (result->s_proto, proto) != 0)
	       continue;
             else
               LOOKUP_NAME (s_name, s_aliases)
           },
           const char *name, const char *proto)

DB_LOOKUP (servbyport,
           {
             /* Must match both port and protocol.  */
             if (result->s_port == port
                 && (proto == NULL
                     || strcmp (result->s_proto, proto) == 0))
               break;
           }, int port, const char *proto)
