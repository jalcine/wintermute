# vim: set ts=2 sts=2 sw=2 fdm=indent
# Uses PkgConfig to look up the package 'libconfig++'.
# Site: http://www.hyperrealm.com/libconfig
###############################################################################
# Author: Jacky Alciné <me@jalcine.me>
#
# Wintermute is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 3 of the License, or (at your option) any later version.
#
# Wintermute is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# Library General Public License for more details.
#
# You should have received a copy of the GNU Library General Public
# License along with Wintermute; if not, write to the
# Free Software Foundation, Inc., 59 Temple Place - Suite 330,
# Boston, MA 02111-1307, USA.
###############################################################################
INCLUDE(FindPkgConfig)
INCLUDE(FindPackageHandleStandardArgs)

PKG_CHECK_MODULES(LIBCONFIG QUIET REQUIRED libconfig++)

FIND_PACKAGE_HANDLE_STANDARD_ARGS(libconfig DEFAULT_MSG
  LIBCONFIG_LIBRARIES LIBCONFIG_FOUND)

MARK_AS_ADVANCED(LIBCONFIG_INCLUDE_DIRS LIBCONFIG_CFLAGS LIBCONFIG_CFLAGS_OTHER
  LIBCONFIG_LIBRARIES LIBCONFIG_LIBRARY_DIR LIBCONFIG_DEFINITIONS)
