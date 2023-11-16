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

#define SERVICE gr
#define SERVICE_r grgid_r
#define STRUCTURE group

#define FIRST_PROTO gid_t gid
#define FIRST_ARG   gid


#define PRINT_ARGS printf ("query: [%d]\n", gid);

#define PRINT_RESULT \
	printf("[%s] [%s] [%d]",                            \
	       result->gr_name, result->gr_passwd,          \
	       result->gr_gid);                             \
        if (result->gr_mem)                                 \
          for (char **p = result->gr_mem; *p != NULL; p++)  \
            printf(" [%s]", *p);                            \
        printf("\n");

#include "tst-getXXXbyYYY_r.c"

int
main(void)
{
  int retval;

  retval = query (0);
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query (65534);
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query (-100);
  if (retval != NSS_STATUS_NOTFOUND)
    return 1;

  return 0;
}
