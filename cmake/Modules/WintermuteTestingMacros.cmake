## TODO: Add proper CMake module definition here.
###############################################################################
### Copyright (C) 2013 Jacky Alcine <jacky.alcine@thesii.org>
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

include(WintermuteMacros)

macro(wintermute_add_unit_test unittestname unittestsrc)
  SET(unittest_${unittestname}_SRCS ${unittestsrc} ${WINTERMUTE_TEST_CORE_SOURCES})
  qt4_automoc(${unittest_${unittestname}_SRCS})

  add_executable(unittest_${unittestname} ${unittest_${unittestname}_SRCS})
  wintermute_add_properties(unittest_${unittestname})
  target_link_libraries(unittest_${unittestname} ${WINTERMUTE_TEST_LIBRARIES})
  
  add_test(unittest_${unittestname} unittest_${unittestname})
  add_dependencies(test unittest_${unittestname})
endmacro(wintermute_add_unit_test unittestname unittestsrc)


