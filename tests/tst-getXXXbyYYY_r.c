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

#include <errno.h>
#include <stdio.h>

#include "nss-econf.h"

#define CONCAT(a,b) CONCAT1(a,b)
#define CONCAT1(a,b) a##b

#ifndef ENTNAME
# define ENTNAME        CONCAT(SERVICE,ent)
#endif

#ifndef STRUCTURE
# define STRUCTURE ENTNAME
#endif

#ifndef SERVICE_r
# define SERVICE_r      CONCAT(SERVICE,byname_r)
#endif

#ifndef FIRST_PROTO
# define FIRST_PROTO    const char *name
# define FIRST_ARG      name
#endif

#ifndef SECOND_PROTO
# define SECOND_PROTO
# define SECOND_ARG
#endif

#ifdef NEED_H_ERRNO
# define H_ERRNO_ARG    , &herrnop
#else
# define H_ERRNO_ARG
#endif


#define MAX_BUF 4096

static int
query (FIRST_PROTO SECOND_PROTO)
{
  struct STRUCTURE res_buf;
  char buf[MAX_BUF] = "";
  size_t buflen = MAX_BUF;
  int errnop;
#ifdef NEED_H_ERRNO
  int herrnop;
#endif
  enum nss_status retval;

  PRINT_ARGS

  retval = CONCAT(_nss_econf_get,SERVICE_r) (FIRST_ARG SECOND_ARG, &res_buf,
					     buf, buflen, &errnop H_ERRNO_ARG);

  if (retval != NSS_STATUS_SUCCESS)
    {
      if (errnop == ERANGE)
	fprintf (stderr, "Buffer(%li) too small\n", buflen);
      else
	fprintf (stderr, "Retval = %i\n", retval);
    }
  else
    {
      struct STRUCTURE *result = &res_buf;

      PRINT_RESULT
    }

  return retval;
}
