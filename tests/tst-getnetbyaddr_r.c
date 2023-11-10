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

#define SERVICE net
#define SERVICE_r netbyaddr_r

#define NEED_H_ERRNO

#define FIRST_PROTO uint32_t net
#define FIRST_ARG   net
#define SECOND_PROTO , int type
#define SECOND_ARG   , type

#define PRINT_ARGS printf ("query: [%u,%i]\n", net, type);

#define PRINT_RESULT \
        printf("[%s] [%d] [%u]", result->n_name,               \
               result->n_addrtype, result->n_net);             \
        if (result->n_aliases)                                 \
          for (char **p = result->n_aliases; *p != NULL; p++)  \
            printf(" [%s]", *p);                               \
        printf("\n");


#include "tst-getXXXbyYYY_r.c"

int
main(void)
{
  int retval;

  retval = query (2851995648, AF_INET);
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query (2851995648, AF_UNSPEC);
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query (0, -1);
  if (retval != NSS_STATUS_NOTFOUND)
    return 1;

  return 0;
}
