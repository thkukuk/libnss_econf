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

#include <pwd.h>

#include "nss-econf.h"

#define ENTNAME         pwent
#define STRUCTURE       passwd
#define DATABASE	"passwd"

#include "files-parse.c"

LINE_PARSER
(,
 /* STRING_FIELD (result->pw_name, ISCOLON, 0); */
 STRING_FIELD (result->pw_passwd, ISCOLON, 0);
 INT_FIELD (result->pw_uid, ISCOLON, 0, 10,);
 INT_FIELD (result->pw_gid, ISCOLON, 0, 10,);
 STRING_FIELD (result->pw_gecos, ISCOLON, 0);
 STRING_FIELD (result->pw_dir, ISCOLON, 0);
 result->pw_shell = line;
)


#define STRUCT_NAME pw_name
#define DELIM ":"

#include "econf-XXX.c"


DB_LOOKUP_KEY (pwnam, name)

DB_LOOKUP (pwuid, '=', 20, ("%lu", (unsigned long int) uid),
           {
             if (result->pw_uid == uid)
               break;
           }, uid_t uid)
