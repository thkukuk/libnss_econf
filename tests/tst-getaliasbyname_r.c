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

#define SERVICE alias

#define PRINT_ARGS printf ("query: [%s]\n", name);

#define PRINT_RESULT \
        printf ("[%s] aliases[%li]=",                              \
                result->alias_name, result->alias_members_len);    \
        if (result->alias_members)                                 \
          for (char **p = result->alias_members; *p != NULL; p++)  \
            printf("[%s] ", *p);                                   \
        printf("\n");

#include "tst-getXXXbyYYY_r.c"

int
main(void)
{
  int retval;

  /* Ubuntu has no alias file */
  if (_nss_econf_setaliasent (-1) == NSS_STATUS_NOTFOUND)
    return 77;

  retval = query ("postmaster");
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query ("daemon");
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query ("doesnotexist");
  if (retval != NSS_STATUS_NOTFOUND)
    return 1;

  return 0;
}
