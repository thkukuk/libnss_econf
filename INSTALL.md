# Building and installing libnss_econf

There is one option to build it: meson

## Building with meson

libeconf requires a relatively recent version of meson, version 0.58 or newer.

Building with Meson is quite simple:

```shell
$ meson setup build
$ meson compile -C build
$ meson test -C build
$ sudo meson install -C build
```

If you want to build with the address sanitizer enabled, add
`-Db_sanitize=address` as an argument to `meson setup build`.

# Tagging new Release

1. Edit NEWS declaring the new version number and making all the changes to it.
2. Update the version number in meson.build.
3. Commit to git.
4. 'Draft a new release' on github.
