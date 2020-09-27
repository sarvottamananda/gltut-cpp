project('glad', 'c',
  version : '1.0',
  default_options : ['warning_level=2', 'c_std=c99'])

# These arguments are only used to build the shared library
# not the executables that use the library.
lib_args = ['-DBUILDING_GLAD', '-ldl', '-Wno-pedantic']

cc =  meson.get_compiler('c')

dl_dep = cc.find_library('dl', required : true)

glad_lib = library('glad', 'glad.c',
  c_args : lib_args,
  gnu_symbol_visibility : 'hidden',
  dependencies : dl_dep
)

# Make this library usable as a Meson subproject.
glad_dep = declare_dependency(
  include_directories: include_directories('.'),
  link_with : glad_lib)

# Make this library usable from the system's
# package manager.

pkg_mod = import('pkgconfig')
pkg_mod.generate(
  name : 'glad',
  filebase : 'glad',
  description : 'Meson sample project.',
  subdirs : 'glad',
  libraries : glad_lib,
  version : '1.0',
)
