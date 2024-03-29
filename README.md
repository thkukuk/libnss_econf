# libnss_econf

This is a NSS module for glibc which can read several databases like
`services` or `rpc` from different places on the filesystem and merge
them to one.

It follows the [Configuration Files Specification](https://github.com/uapi-group/specifications/blob/main/specs/configuration_files_specification.md) of the
[Linux Userspace API Group](https://uapi-group.org/).

This module can be used as a 1:1 replacement of the libnss_files module from glibc.

## Services

The following services are supported:

  * **aliases** - Mail aliases, used by [getaliasent(3)](https://manpages.opensuse.org/getaliasent.3) and related functions.
  * **group** - Groups of users, used by [getgrent(3)](https://manpages.opensuse.org/getgrent.3) and related functions.
  * **networks** - Network names and numbers, used by [getnetent(3)](https://manpages.opensuse.org/getnetent.3) and related functions.
  * **passwd** - User passwords, used by [getpwent(3)](https://manpages.opensuse.org/getpwent.3) and related functions.
  * **protocols** - Network protocols, used by [getprotoent(3)](https://manpages.opensuse.org/getprotoent.3) and related functions.
  * **rpc** - Remote procedure call names and numbers, used by [getrpcent(3)](https://manpages.opensuse.org/getrpcent.3) and related functions.
  * **services** - Network services, used by [getservent(3)](https://manpages.opensuse.org/getservent.3) and related functions.
  * **shadow** - Shadow user passwords, used by [getspnam(3)](https://manpages.opensuse.org/getspnam.3) and related functions.


For **group**, **passwd** and **shadow** the +/- entries are not supported.

Keys in the maps needs to be unique, if several entries in a file have the same name, it's undefined which one will be returned.
As the entries in the *services* file do not have a single unique key, but each entry is identified by two keys, the handling of this file is somewhat special: overwriting existing entries only works with the functions [getservbyname(3)](https://manpages.opensuse.org/getservbyname.3) and [getservbyport(3)](https://manpages.opensuse.org/getservbyport.3), [getservent(3)](https://manpages.opensuse.org/getservent.3) lists both entries, the old and the new one.

Not supported are *ethers*, *initgroups*, *netgroup* and *publickey*.

## Usage

List the module in the [nsswitch.conf](https://manpages.opensuse.org/nsswitch.conf) configuration file.
For example:

```
    passwd:     econf systemd
    group:      econf systemd

    hosts:      files dns
    networks:   econf

    aliases:    econf
    ethers:     files
    netgroup:   files
    protocols:  econf
    publickey:  files
    rpc:        econf
    services:   econf

    automount:  files
    bootparams: files
    netmasks:   files

```

## Building and installing libnss_econf

### Building

libnss_econf requires [libeconf](https://manpages.opensuse.org/libeconf) in
version 0.4.9 or newer and meson version 0.58 or newer.

Building with meson is quite simple:

```shell
$ meson setup build
$ meson compile -C build
$ meson test -C build
```

If you want to build with the address sanitizer enabled, add
`-Db_sanitize=address` as an argument to `meson setup build`.

### Installing

```shell
$ sudo meson install -C build
```

## Licensing

See the file COPYING for full text and copying restrictions.
