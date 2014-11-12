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

option(GENERATE_DOCUMENTATION OFF "Generate documentation using Doxygen.")

if (GENERATE_DOCUMENTATION EQUAL OFF)
  return()
endif()

find_package(Doxygen 1.8.8 REQUIRED)

if (NOT DOXYGEN_FOUND)
  message("Cannot generate documentation, Doxygen not found.")
endif()

# Look for dot for graphing.
if (NOT DOXYGEN_DOT_FOUND)
  find_program(DOT_PROGRAM dot
    DOC "Graphviz application for drawing graphs.")

  if (DOT_PROGRAM-NOTFOUND)
    message("Cannot find Graphviz; no graphs will be drawn in documentation.")
  else()
    set(DOXYGEN_DOT_EXECUTABLE ${DOT_PROGRAM})
  endif()
endif()
