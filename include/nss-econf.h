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

#ifndef _NSS_ECONF_H
#define _NSS_ECONF_H     1

#include <aliases.h>
#include <nss.h>
#include <netdb.h>
#include <grp.h>
#include <pwd.h>
#include <shadow.h>
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

extern enum nss_status _nss_econf_setgrent (int stayopen);
extern enum nss_status _nss_econf_endgrent (void);
extern enum nss_status _nss_econf_getgrent_r (struct group *result,
                char *buffer, size_t buflen, int *errnop);
extern enum nss_status _nss_econf_getgrnam_r (const char *name,
                struct group *result, char *buffer, size_t buflen,
                int *errnop);
extern enum nss_status _nss_econf_getgrgid_r (gid_t gid,
                struct group *result, char *buffer, size_t buflen,
                int *errnop);

extern enum nss_status _nss_econf_sethostent (int stayopen);
extern enum nss_status _nss_econf_endhostent (void);
extern enum nss_status _nss_econf_gethostent_r (struct hostent *result,
		char *buffer, size_t buflen, int *errnop, int *herrnop);
extern enum nss_status _nss_econf_gethostbyaddr_r (const void *addr,
		socklen_t addrlen, int af, struct hostent *host,
		char *buffer, size_t buflen, int *errnop, int *h_errnop);
extern enum nss_status _nss_econf_gethostbyname_r (const char *name,
                struct hostent *result, char *buffer, size_t buflen,
                int *errnop, int *herrnop);
extern enum nss_status _nss_econf_gethostbyname2_r (const char *name, int af,
                struct hostent *result, char *buffer, size_t buflen,
                int *errnop, int *herrnop);

extern enum nss_status _nss_econf_setnetent (int stayopen);
extern enum nss_status _nss_econf_endnetent (void);
extern enum nss_status _nss_econf_getnetent_r (struct netent *result,
		char *buffer, size_t buflen, int *errnop, int *herrnop);
extern enum nss_status _nss_econf_getnetbyname_r (const char *name,
		struct netent *result, char *buffer, size_t buflen,
		int *errnop, int *herrnop);
extern enum nss_status _nss_econf_getnetbyaddr_r (uint32_t net, int type,
		struct netent *result, char *buffer, size_t buflen,
	        int *errnop , int *herrnop);

extern enum nss_status _nss_econf_getnetbyname_r (const char *name,
		struct netent *result, char *buffer, size_t buflen,
		int *errnop, int *herrnop);

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

extern enum nss_status _nss_econf_setpwent (int stayopen);
extern enum nss_status _nss_econf_endpwent (void);
extern enum nss_status _nss_econf_getpwent_r (struct passwd *result,
		char *buffer, size_t buflen, int *errnop);
extern enum nss_status _nss_econf_getpwnam_r (const char *name,
		struct passwd *result, char *buffer, size_t buflen,
	        int *errnop);
extern enum nss_status _nss_econf_getpwuid_r (uid_t uid,
		struct passwd *result, char *buffer, size_t buflen,
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

extern enum nss_status _nss_econf_setspent (int stayopen);
extern enum nss_status _nss_econf_endspent (void);
extern enum nss_status _nss_econf_getspent_r (struct spwd *result,
		char *buffer, size_t buflen, int *errnop);
extern enum nss_status _nss_econf_getspnam_r (const char *name,
		struct spwd *result, char *buffer, size_t buflen,
	        int *errnop);

#endif
