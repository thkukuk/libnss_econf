# libnss_econf

This is a NSS module for glibc which can read several databases like
`services` or `rpc` from different places on the filesystem and merge
them to one.

It follows the [Configuration Files Specification](https://github.com/uapi-group/specifications/blob/main/specs/configuration_files_specification.md) of the 
[Linux Userspace API Group](https://uapi-group.org/).

## Usage

List the module in the `nsswitch.conf` configuration file.
For example:

```
    passwd:     compat systemd
    group:      compat systemd

    hosts:      files dns
    networks:   files dns

    aliases:    econf
    ethers:     files usrfiles
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

There is one option to build it: meson

### Building

libnss_econf requires a relatively recent version of meson, version 0.58 or newer.

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

### Tagging new Release

1. Edit NEWS declaring the new version number and making all the changes to it.
2. Update the version number in meson.build.
3. Commit to git.
4. 'Draft a new release' on github.

## Licensing

See the file COPYING for full text and copying restrictions.
