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

#define SERVICE serv
#define SERVICE_r servbyport_r

#define FIRST_PROTO  int port
#define FIRST_ARG    ntohs(port)
#define SECOND_PROTO , const char *proto
#define SECOND_ARG   , proto

#define PRINT_ARGS printf ("query: [%i,%s]\n", port, proto);

#define PRINT_RESULT \
        printf("[%s] [%s/%d]",                                    \
               result->s_name, result->s_proto,                   \
               ntohs(result->s_port));                            \
        if (result->s_aliases)                                    \
          for (char **p = result->s_aliases; *p != NULL; p++)     \
            printf(" [%s]", *p);                                  \
        printf("\n");

#include "tst-getXXXbyYYY_r.c"

int
main(void)
{
  int retval;

  retval = query (111, NULL);
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query (111, "tcp");
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query (111, "udp");
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query (111, "none");
  if (retval != NSS_STATUS_NOTFOUND)
    return 1;

  return 0;
}
