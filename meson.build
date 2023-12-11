project(
  'libnss_econf',
  'c',
  meson_version : '>= 0.58.0',
  default_options : ['buildtype=debugoptimized',
  		  'default_library=shared',
                  'c_std=gnu11',
                  'prefix=/usr',
                  'sysconfdir=/etc',
		  'b_pie=true',
                  'b_lto=true',
		  'warning_level=2',],
  license : 'LGPLv2+',
  version : '0.0.1',
)

conf = configuration_data()
conf.set_quoted('USRDIR', get_option('usr-dir'))
conf.set('_GNU_SOURCE', 1)

cc = meson.get_compiler('c')
pkg = import('pkgconfig')
inc = include_directories('include')

possible_cc_flags = [
		  '-D_FORTIFY_SOURCE=2',
		  '-fstack-protector-strong',
		  '-funwind-tables',
		  '-fasynchronous-unwind-tables',
		  '-fstack-clash-protection',
		  '-Werror=return-type',
		  '-Wbad-function-cast',
		  '-Wcast-align',
		  '-Wcast-qual',
		  '-Winline',
		  '-Wmissing-declarations',
		  '-Wmissing-prototypes',
		  '-Wnested-externs',
		  '-Wshadow',
		  '-Wstrict-prototypes',
		  '-Wundef',
		  ]
add_project_arguments(cc.get_supported_arguments(possible_cc_flags), language : 'c')

libnss_econf_src = files(
  'src/aliases.c',
  'src/group.c',
  'src/hosts.c',
  'src/internal.c',
  'src/networks.c',
  'src/passwd.c',
  'src/proto.c',
  'src/rpc.c',
  'src/services.c',
  'src/shadow.c'
)

mapfile = 'src/libnss_econf.map'
version_flag = '-Wl,--version-script,@0@/@1@'.format(meson.current_source_dir(), mapfile)

libeconf_dep = dependency('libeconf', version : '>= 0.4.9')

if cc.has_function('econf_readConfig', prefix : '#include <libeconf.h>')
  conf.set('HAVE_ECONF_READCONFIG', 1)
endif

lib = library(
  'nss_econf',
  libnss_econf_src,
  include_directories : inc,
  install : true,
  link_args : version_flag,
  link_depends : mapfile,
  version : meson.project_version(),
  soversion : '2',
  dependencies : [libeconf_dep, ],
)

pkg.generate(
  lib,
  name : 'libnss_econf',
  description : 'NSS plugin using libeconf to read files like services, rpc, ...',
  version : meson.project_version(),
)

libnss_econf_dep = declare_dependency(
  link_with : lib,
  include_directories : inc,
)

config_h = configure_file(
                 output : 'config.h',
                 configuration : conf)

# Unit tests
subdir('tests')
