# libnss_econf
NSS module for glibc using libeconf to read all files
NSS econf module
================

This is a NSS module which can read several databases like `services` or
`rpc` from different places on the filesystem and merge them to one.

It follows the [Configuration Files Specification](https://github.com/uapi-group/specifications/blob/main/specs/configuration_files_specification.md)

Usage
-----

List the module in the `nsswitch.conf` configuration file.
For example:

```
    passwd:     compat systemd
    group:      compat systemd

    hosts:      files dns
    networks:   files dns

    aliases:    files usrfiles
    ethers:     files usrfiles
    netgroup:   files nis
    protocols:  files usrfiles
    publickey:  files
    rpc:        files usrfiles
    services:   econf

    automount:  files nis
    bootparams: files
    netmasks:   files

```


Licensing
---------

See the file COPYING for full text and copying restrictions.
