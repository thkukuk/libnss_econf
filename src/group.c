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

#define ENTNAME         grent
#define STRUCTURE       group
#define DATABASE	"group"

#define TRAILING_LIST_MEMBER            gr_mem
#define TRAILING_LIST_SEPARATOR_P(c)    ((c) == ',')
#include "files-parse.c"
LINE_PARSER
(,
 /* STRING_FIELD (result->gr_name, ISCOLON, 0); */
 STRING_FIELD (result->gr_passwd, ISCOLON, 0);
 INT_FIELD (result->gr_gid, ISCOLON, 0, 10,);
)


#define STRUCT_NAME gr_name
#define DELIM ":"

#include "econf-XXX.c"

DB_LOOKUP_KEY (grnam, name)

DB_LOOKUP (grgid, '=', 20, ("%lu", (unsigned long int) gid),
           {
             if (result->gr_gid == gid)
               break;
           }, gid_t gid)
