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
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nss-econf.h"

#define MAX_BUF 4096

int
main(void)
{
  char buf[MAX_BUF] = "";
  size_t buflen = MAX_BUF;
  struct servent *result = NULL;
  int errnop;
  int retval;

  if (_nss_econf_setservent(-1) != NSS_STATUS_SUCCESS)
    {
      fprintf (stderr, "_nss_econf_setservent() failed\n");
      return 1;
    }

  do {
    struct servent res_buf = {NULL, NULL, 0, NULL};
    retval = _nss_econf_getservent_r (&res_buf, buf, buflen, &errnop);

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

	printf("[%s] [%s/%d]",
	       result->s_name, result->s_proto,
	       ntohs(result->s_port));
	if (result->s_aliases)
	  for (char **p = result->s_aliases; *p != NULL; p++)
	    printf(" [%s]", *p);
	printf("\n");
      }
  }
  while (retval == NSS_STATUS_SUCCESS);

  if (_nss_econf_endservent() != NSS_STATUS_SUCCESS)
    return 1;

  return 0;
}
