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

#define SERVICE proto

#define PRINT_ARGS printf ("query: [%s]\n", name);

#define PRINT_RESULT \
        printf("[%s] [%d]", result->p_name, result->p_proto);  \
        if (result->p_aliases)                                 \
          for (char **p = result->p_aliases; *p != NULL; p++)  \
            printf(" [%s]", *p);                               \
        printf("\n");

#include "tst-getXXXbyYYY_r.c"

int
main(void)
{
  int retval;

  retval = query ("icmp");
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query ("ICMP");
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query ("ipv6");
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query ("IPv6");
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query ("doesnotexist");
  if (retval != NSS_STATUS_NOTFOUND)
    return 1;

  return 0;
}
