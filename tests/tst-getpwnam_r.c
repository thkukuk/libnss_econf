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


#define SERVICE pw
#define SERVICE_r pwnam_r
#define STRUCTURE passwd

#define PRINT_ARGS printf ("query: [%s]\n", name);

#define PRINT_RESULT \
	printf("[%s] [%s] [%d] [%d] [%s] [%s] [%s]\n", \
	       result->pw_name, result->pw_passwd, \
	       result->pw_uid, result->pw_gid, result->pw_gecos,	\
	       result->pw_dir, result->pw_shell);

#include "tst-getXXXbyYYY_r.c"

int
main(void)
{
  int retval;

  retval = query ("root");
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query ("nobody");
  if (retval != NSS_STATUS_SUCCESS)
    return 1;

  retval = query ("doesnotexist");
  if (retval != NSS_STATUS_NOTFOUND)
    return 1;

  return 0;
}
