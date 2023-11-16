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

#include "nss-econf.h"

#define ENTNAME         protoent
#define DATABASE	"protocols"

#define TRAILING_LIST_MEMBER            p_aliases
#define TRAILING_LIST_SEPARATOR_P       isspace
#include "files-parse.c"
LINE_PARSER
("#",
 /* STRING_FIELD (result->p_name, isspace, 1); */
 INT_FIELD (result->p_proto, isspace, 1, 10,);
 )

#define STRUCT_NAME p_name
#define DELIM " \t"

#include "econf-XXX.c"

DB_LOOKUP (protobyname, '.', 0, ("%s", name),
           LOOKUP_NAME (p_name, p_aliases),
           const char *name)

DB_LOOKUP (protobynumber, '=', 20, ("%zd", (ssize_t) proto),
           {
             if (result->p_proto == proto)
               break;
           }, int proto)
