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

#ifndef _NSS_ECONF_H
#define _NSS_ECONF_H     1

#include <aliases.h>
#include <nss.h>
#include <netdb.h>
#include <libeconf.h>

struct data_t
{
  econf_file *key_file;
  char **keys;
  size_t key_number;
  size_t next_key;
};

extern void internal_endent (struct data_t *data);
extern enum nss_status internal_setent (struct data_t *data,
		const char *database, const char *delim);
extern enum nss_status internal_setent_nokeys (struct data_t *data,
                const char *database, const char *delim);

extern enum nss_status _nss_econf_setaliasent (int stayopen);
extern enum nss_status _nss_econf_endaliasent (void);
extern enum nss_status _nss_econf_getaliasent_r (struct aliasent *result,
		char *buffer, size_t buflen, int *errnop);
extern enum nss_status _nss_econf_getaliasbyname_r (const char *name,
		struct aliasent *result, char *buffer, size_t buflen,
		int *errnop);

extern enum nss_status _nss_econf_setprotoent (int stayopen);
extern enum nss_status _nss_econf_endprotoent (void);
extern enum nss_status _nss_econf_getprotoent_r (struct protoent *result,
		char *buffer, size_t buflen, int *errnop);
extern enum nss_status _nss_econf_getprotobyname_r (const char *name,
		struct protoent *result, char *buffer, size_t buflen,
		int *errnop);
extern enum nss_status _nss_econf_getprotobynumber_r (int proto,
		struct protoent *result, char *buffer, size_t buflen,
		int *errnop);

extern enum nss_status _nss_econf_setrpcent (int stayopen);
extern enum nss_status _nss_econf_endrpcent (void);
extern enum nss_status _nss_econf_getrpcent_r (struct rpcent *result,
					       char *buffer, size_t buflen,
					       int *errnop);
extern enum nss_status _nss_econf_getrpcbyname_r (const char *name,
						  struct rpcent *result,
						  char *buffer, size_t buflen,
						  int *errnop);
extern enum nss_status _nss_econf_getrpcbynumber_r (int number,
						    struct rpcent *result,
						    char *buffer,
						    size_t buflen,
						    int *errnop);


extern enum nss_status _nss_econf_setservent (int stayopen);
extern enum nss_status _nss_econf_getservent_r (struct servent *result,
						char *buffer,
						size_t buflen,
						int *errnop);
extern enum nss_status _nss_econf_endservent (void);
extern enum nss_status _nss_econf_getservbyname_r (const char *name,
						   const char *protocol,
						   struct servent *serv,
						   char *buffer,
						   size_t buflen,
						   int *errnop);
extern enum nss_status _nss_econf_getservbyport_r (int port,
						   const char *protocol,
						   struct servent *serv,
						   char *buffer,
						   size_t buflen,
						   int *errnop);
extern enum nss_status _nss_econf_getservbyname_r (const char *name,
						   const char *proto,
						   struct servent *result,
						   char *buffer,
						   size_t buflen,
						   int *errnop);

#endif
