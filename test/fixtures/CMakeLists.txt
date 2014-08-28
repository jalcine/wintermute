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

ADD_LIBRARY(wintermutesample SHARED ./wintermute_sample.cc)
TARGET_LINK_LIBRARIES(wintermutesample wintermutecore)
WINTERMUTE_LINK_LIBRARIES(wintermutesample)
WINTERMUTE_ADD_TARGET_PROPERTIES(wintermutesample)
TARGET_INCLUDE_DIRECTORIES(wintermutesample PUBLIC
	${CMAKE_SOURCE_DIR}/src
	${CMAKE_SOURCE_DIR}/test/include
  ${CMAKE_BINARY_DIR}/src/libwintermutecore)

# TODO Export file path to the library.
