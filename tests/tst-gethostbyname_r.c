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

#include <arpa/inet.h>

#define SERVICE host
#define SERVICE_r hostbyname_r

#define NEED_H_ERRNO

#define FIRST_PROTO const char *name
#define FIRST_ARG   name

#define PRINT_ARGS  printf ("query: [%s]\n", name);

#define PRINT_RESULT \
        printf("[%s]", result->h_name); \
        if (result->h_aliases)                                 \
          for (char **p = result->h_aliases; *p != NULL; p++)  \
            printf(" [%s]", *p);                               \
        if (result->h_addrtype != AF_INET)                     \
          printf("Not an IPv4 internet address.");             \
        else                                                   \
          for (char **p = result->h_addr_list; *p; p++)        \
            printf(" %s", inet_ntoa((*(struct in_addr *)*p))); \
        printf("\n");

#include "tst-getXXXbyYYY_r.c"

int
main(void)
{
  int retval;

  retval = query ("localhost");
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query ("doesnot.exist");
  if (retval != NSS_STATUS_NOTFOUND)
    return 1;

  return 0;
}
