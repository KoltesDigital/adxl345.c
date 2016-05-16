import os

headers = ['adxl345.h']
sources = ['adxl345.c']

lib = Library('libadxl345', sources)

install_prefix = '/usr/local'
install_include = os.path.join(install_prefix, 'include')
install_lib = os.path.join(install_prefix, 'lib')

Alias('install', Install(install_include, headers))
Alias('install', Install(install_lib, lib))
Command('uninstall', None, Delete(FindInstalledFiles()))

Default(lib)
