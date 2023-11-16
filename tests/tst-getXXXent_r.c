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

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nss.h>

#include "nss-econf.h"

#define CONCAT(a,b) CONCAT1(a,b)
#define CONCAT1(a,b) a##b

#ifndef STRUCTURE
# define STRUCTURE ENTNAME
#endif

#define ENTNAME_r       CONCAT(ENTNAME,_r)

#ifdef NEED_H_ERRNO
# define H_ERRNO_ARG    , &herrnop
#else
# define H_ERRNO_ARG
#endif

#define MAX_BUF 4096

int
main(void)
{
  char buf[MAX_BUF] = "";
  size_t buflen = MAX_BUF;
  struct STRUCTURE *result = NULL;
  int errnop;
#ifdef NEED_H_ERRNO
  int herrnop;
#endif
  enum nss_status retval;

  if ((retval = CONCAT(_nss_econf_set,ENTNAME)(-1)) != NSS_STATUS_SUCCESS)
    {
      if (retval == NSS_STATUS_NOTFOUND)
	{
	  fprintf (stderr, "_nss_econf_setENTNAME(): config file not found\n");
	  return 77; /* Looks like there is no config on this system */
	}
      fprintf (stderr, "_nss_econf_setENTNAME() failed: %i\n", retval);
      return 1;
    }

  do {
    struct STRUCTURE res_buf;
    retval = CONCAT(_nss_econf_get,ENTNAME_r) (&res_buf, buf, buflen,
					       &errnop H_ERRNO_ARG);

    if (retval != NSS_STATUS_SUCCESS)
      {
	if (errnop == ERANGE)
	  fprintf (stderr, "Buffer(%li) too small\n", buflen);
	else
	  fprintf (stderr, "Retval = %i\n", retval);
      }
    else
      {
	result = &res_buf;

	PRINT_RESULT
      }
  }
  while (retval == NSS_STATUS_SUCCESS);

  if (CONCAT(_nss_econf_end,ENTNAME) () != NSS_STATUS_SUCCESS)
    return 1;

  return 0;
}
