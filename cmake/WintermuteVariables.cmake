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
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

if (NOT DEFINED _wntr_vars)
  set(_wntr_vars ON)
else()
  return()
endif()

SET(WINTERMUTE_COMPILE_FLAGS
	-Wall
	-Wpedantic
	-fstrict-aliasing
	-std=c++11
  -Wc++11-compat
	-Wabi
)

SET(WINTERMUTE_COMPILE_FLAGS_DEBUG
	-O0
  -Wenum-compare
  -Wuseless-cast
  -Wextra
	-Waggressive-loop-optimizations
  -Wno-conversion-null
	-Wctor-dtor-privacy
	-Wno-deprecated
	-Wno-unused-parameter
	-g
)
SET(WINTERMUTE_COMPILE_FLAGS_RELEASE
	-O3
)

set(WINTERMUTE_COMPILE_FLAGS ${WINTERMUTE_COMPILE_FLAGS})
set(WINTERMUTE_COMPILE_FLAGS_DEBUG "${WINTERMUTE_COMPILE_FLAGS_DEBUG}")
set(WINTERMUTE_COMPILE_FLAGS_RELEASE "${WINTERMUTE_COMPILE_FLAGS_RELEASE}")

SET(WINTERMUTE_COMPILE_DEFINITIONS "WINTERMUTE")
SET(WINTERMUTE_COMPILE_DEFINITIONS_DEBUG "WINTERMUTE_DEBUG")