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

#define ENTNAME         spent
#define STRUCTURE       spwd
#define DATABASE	"shadow"

#define FALSEP(arg) 0

#include "files-parse.c"

LINE_PARSER
(,
 /* STRING_FIELD (result->sp_name, ISCOLON, 0); */
  STRING_FIELD (result->sp_pwdp, ISCOLON, 0);
  INT_FIELD_MAYBE_NULL (result->sp_lstchg, ISCOLON, 0, 10, (long int) (int),
                        (long int) -1);
  INT_FIELD_MAYBE_NULL (result->sp_min, ISCOLON, 0, 10, (long int) (int),
                        (long int) -1);
  INT_FIELD_MAYBE_NULL (result->sp_max, ISCOLON, 0, 10, (long int) (int),
                        (long int) -1);
  while (isspace (*line))
    ++line;
  if (*line == '\0')
    {
      /* The old form.  */
      result->sp_warn = -1l;
      result->sp_inact = -1l;
      result->sp_expire = -1l;
      result->sp_flag = ~0ul;
    }
  else
    {
      INT_FIELD_MAYBE_NULL (result->sp_warn, ISCOLON, 0, 10,
                            (long int) (int), (long int) -1);
      INT_FIELD_MAYBE_NULL (result->sp_inact, ISCOLON, 0, 10,
                            (long int) (int), (long int) -1);
      INT_FIELD_MAYBE_NULL (result->sp_expire, ISCOLON, 0, 10,
                            (long int) (int), (long int) -1);
      if (*line != '\0')
        INT_FIELD_MAYBE_NULL (result->sp_flag, FALSEP, 0, 10,
                              (unsigned long int), ~0ul)
      else
        result->sp_flag = ~0ul;
    }
)

#define STRUCT_NAME sp_namp
#define DELIM ":"

#include "econf-XXX.c"

DB_LOOKUP_KEY (spnam, name)
