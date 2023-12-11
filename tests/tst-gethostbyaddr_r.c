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

#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define SERVICE host
#define SERVICE_r hostbyaddr_r

#define NEED_H_ERRNO

#define FIRST_PROTO const void *addr
#define FIRST_ARG   addr

#define SECOND_PROTO ,socklen_t addrlen, int af
#define SECOND_ARG ,addrlen, af

#define PRINT_ARGS /* printf ("query: [%i]\n", number); */

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
  struct in_addr addr;
  int retval;

  inet_aton("127.0.0.1", &addr);

  printf ("query: [127.0.0.1]\n");
  retval = query (&addr, sizeof (addr), AF_INET);
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  printf ("query: [127.0.255.1]\n");
  inet_aton("127.0.255.1", &addr);
  retval = query (&addr, sizeof (addr), AF_INET);
  if (retval != NSS_STATUS_NOTFOUND)
    return 1;

  printf ("query: [NULL]\n");
  retval = query (0, 0, AF_INET);
  if (retval != NSS_STATUS_NOTFOUND)
    return 1;

  return 0;
}
