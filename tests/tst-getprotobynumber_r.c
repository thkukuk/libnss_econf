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

#define SERVICE proto
#define SERVICE_r protobynumber_r

#define FIRST_PROTO int proto
#define FIRST_ARG   proto

#define PRINT_ARGS printf ("query: [%i]\n", proto);

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

  retval = query (4);
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query (41);
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query (58);
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query (200);
  if (retval != NSS_STATUS_NOTFOUND)
    return 1;

  return 0;
}
