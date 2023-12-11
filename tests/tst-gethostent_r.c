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

#define ENTNAME hostent
#define NEED_H_ERRNO

#define PRINT_RESULT \
	printf("[%s]", result->h_name); \
	if (result->h_aliases)                                 \
	  for (char **p = result->h_aliases; *p != NULL; p++)  \
	    printf(" [%s]", *p);                               \
	if (result->h_addrtype != AF_INET) 		       \
	  printf("Not an IPv4 internet address.");             \
	else                                                   \
          for (char **p = result->h_addr_list; *p; p++)        \
            printf(" %s", inet_ntoa((*(struct in_addr *)*p))); \
	printf("\n");

#include "tst-getXXXent_r.c"
