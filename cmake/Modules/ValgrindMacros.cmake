###############################################################################
### Copyright (C) 2013 Jacky Alciné <me@jalcine.me>
###
### This file is free software; you can redistribute it and/or modify
### it under the terms of the GNU General Public License as published by
### the Free Software Foundation; either version 3 of the License, or
### (at your option) any later version.
###
### This file is distributed in the hope that it will be useful,
### but WITHOUT ANY WARRANTY; without even the implied warranty of
### MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
### GNU General Public License for more details.
###
### You should have received a copy of the GNU General Public License
### along with this file.  If not, see <http://www.gnu.org/licenses/>.
###############################################################################

find_file(VALGRIND_PATH valgrind)

if (NOT DEFINED ${VALGRIND_PATH})
  message(WARNING "Valgrind not found.")
  return()
endif()

macro(valgrind_memorycheck_target _target)
  set(_valgrind_target "valgrind_memchk_${_target}")

  add_custom_target(${_valgrind_target} DEPENDS ${_target})

  add_custom_command(TARGET ${_valgrind_target}
    COMMENT "Checking for memory leaks (Ctrl+C/Cmd+C if it hangs)..."
    COMMAND ${VALGRIND_PATH} --tool=memcheck --leak-check=yes --show-reachable=yes --num-callers=20 --track-fds=yes "${_target}"
  )
endmacro(valgrind_memorycheck_target _target)
