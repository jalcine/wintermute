# vim: set ts=2 sts=2 sw=2 fdm=indent
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
PROJECT(WintermuteTestFixtures)
INCLUDE(WintermutePlugin)

# TODO: Create dynamic fixture library to drop into tests.

set(_srcs
  ./wintermute_sample.cc
)

ADD_LIBRARY(wintermute-sample-plugin SHARED ${_srcs})

WINTERMUTE_PLUGIN_DECLARE(
  AUTO_INSTALL OFF
  TARGET
    wintermute-sample-plugin
  INCLUDE_DIRECTORIES
    ${WINTERMUTE_TEST_INCLUDE_DIRS}
  )
