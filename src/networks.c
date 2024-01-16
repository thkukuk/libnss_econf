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

#include "nss-econf.h"

#define ENTNAME         netent
#define DATABASE	"networks"
#define NEED_H_ERRNO

#define TRAILING_LIST_MEMBER            n_aliases
#define TRAILING_LIST_SEPARATOR_P       isspace
#include "files-parse.c"
LINE_PARSER
("#",
 {
   char *addr;
   char *cp;
   int n = 1;

   /* STRING_FIELD (result->n_name, isspace, 1); */

   STRING_FIELD (addr, isspace, 1);
   /* 'inet_network' does not add zeroes at the end if the network number
      does not four byte values.  We add them ourselves if necessary.  */
   cp = strchr (addr, '.');
   if (cp != NULL)
     {
       ++n;
       cp = strchr (cp + 1, '.');
       if (cp != NULL)
         {
           ++n;
           cp = strchr (cp + 1, '.');
           if (cp != NULL)
             ++n;
         }
     }
   if (n < 4)
     {
       char *newp = (char *) alloca (strlen (addr) + (4 - n) * 2 + 1);
       cp = stpcpy (newp, addr);
       do
         {
           *cp++ = '.';
           *cp++ = '0';
         }
       while (++n < 4);
       *cp = '\0';
       addr = newp;
     }
   result->n_net = inet_network (addr);
   result->n_addrtype = AF_INET;

 })

#define STRUCT_NAME n_name
#define DELIM " \t"
#define NEED_GETENT

#include "econf-XXX.c"

DB_LOOKUP (netbyname,
           LOOKUP_NAME_CASE (n_name, n_aliases),
           const char *name)

DB_LOOKUP (netbyaddr,
           {
             if ((type == AF_UNSPEC || result->n_addrtype == type)
                 && result->n_net == net)
               /* Bingo!  */
               break;
           }, uint32_t net, int type)
