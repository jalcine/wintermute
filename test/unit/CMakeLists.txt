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

## Make sure we can automoc the tests.
include_directories ( $ {CMAKE_CURRENT_BINARY_DIR} )

## Produce the list of buildable tests.
set ( WINTERMUTE_TEST_UNITS
      clean_start
      command_line
#plugin_factory
    )

## Add each unit test to the test suite.
foreach ( WINTERMUTE_TEST_UNIT $ {WINTERMUTE_TEST_UNITS} )
wintermute_add_unit_test ( $ {WINTERMUTE_TEST_UNIT} "${CMAKE_CURRENT_SOURCE_DIR}/${WINTERMUTE_TEST_UNIT}.cpp" )
endforeach ( WINTERMUTE_TEST_UNIT $ {WINTERMUTE_TEST_UNITS} )

