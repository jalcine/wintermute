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

macro(add_a_unit_test _name)
  add_a_test(unit "${_name}" "${CMAKE_SOURCE_DIR}/test/unit/${_name}.hh")
endmacro(add_a_unit_test)

add_a_unit_test(singleton)
add_a_unit_test(message)
add_a_unit_test(module_designation)
add_a_unit_test(module)
add_a_unit_test(module_pool)
add_a_unit_test(tunnel)
add_a_unit_test(util_serializable)
add_a_unit_test(dispatcher)
