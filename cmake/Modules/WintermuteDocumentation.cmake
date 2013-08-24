# TODO: Add proper CMake module header definitions.
###############################################################################
### Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
###
### This file is part of Wintermute, the extensible AI platform.
###
### Wintermute is free software; you can redistribute it and/or modify
### it under the terms of the GNU General Public License as published by
### the Free Software Foundation; either version 3 of the License, or
### (at your option) any later version.
###
### Wintermute is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
### GNU General Public License for more details.
###
### You should have received a copy of the GNU General Public License
### along with Wintermute.  If not, see <http://www.gnu.org/licenses/>.
###############################################################################

find_package(Doxygen REQUIRED)

macro(wintermute_generate_documentation)
  set(_singleArgs TARGETS)
  set(_multiArgs SOURCES)

  cmake_parse_arguments(wgd "" "" "${_multiArgs}" ${ARGN})

  # TODO: Configure the Doxygen configuration file.
  # TODO: Define a target for Doxygen to execute.
  # TODO: Make the documentation target dependent on a parent target.

endmacro(wintermute_generate_documentation)
