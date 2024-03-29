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

#define STRUCT_NAME r_name
#define DELIM " \t"
#define NEED_GETENT

#include "econf-XXX.c"

DB_LOOKUP (rpcbyname,
           LOOKUP_NAME (r_name, r_aliases),
           const char *name)

DB_LOOKUP (rpcbynumber,
           {
             if (result->r_number == number)
               break;
           }, int number)
