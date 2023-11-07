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

#include "config.h"
#include "nss-econf.h"

void
internal_endent (struct data_t *data)
{
  if (data->key_file != NULL)
    {
      econf_free (data->key_file);
      data->key_file = NULL;
    }
  if (data->keys)
    {
      econf_free (data->keys);
      data->keys = NULL;
    }
  data->key_number = 0;
  data->next_key = 0;
}

enum nss_status
internal_setent_nokeys (struct data_t *data, const char *database,
			const char *delim)
{
  enum nss_status status = NSS_STATUS_SUCCESS;

  if (data->key_file == NULL)
    {
      econf_err error = econf_readDirs (&(data->key_file), USRDIR, "/etc",
					database, NULL, delim, "#");
      if (error)
	return NSS_STATUS_UNAVAIL;

      data->keys = NULL;
      data->next_key = 0;
    }
  else
    {
      internal_endent (data);
      status = internal_setent_nokeys (data, database, delim);
    }

  return status;
}


enum nss_status
internal_setent (struct data_t *data, const char *database, const char *delim)
{
  enum nss_status status = NSS_STATUS_SUCCESS;

  if (data->key_file == NULL)
    {
      status = internal_setent_nokeys (data, database, delim);
      if (status != NSS_STATUS_SUCCESS)
	return status;

      econf_err error = econf_getKeys (data->key_file, NULL,
				       &(data->key_number), &(data->keys));

      if (error)
	return NSS_STATUS_UNAVAIL;

      data->next_key = 0;
    }
  else
    {
      internal_endent (data);
      status = internal_setent (data, database, delim);
    }

  return status;
}
