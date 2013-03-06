# TODO: Add default CMake banner here.
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

# Only execute the following block if the 'uninstall' target doesn't exist.
if (NOT TARGET uninstall)
  # Check if the template exists.
  find_file(CMAKE_UNINSTALL_TEMPLATE_FILE cmake_uninstall.cmake.in
            PATHS ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}
            DOC "Path to the CMake uninstallation template.")

  # Proceed only if the file exists.         
  if (EXISTS "${CMAKE_UNINSTALL_TEMPLATE_FILE}")
    # Configure the file accordingly.
    configure_file(${CMAKE_UNINSTALL_TEMPLATE_FILE} 
                  "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake" @ONLY)

    # Define the 'uninstall' target.
    add_custom_target(uninstall
      "${CMAKE_COMMAND}" -P "${CMAKE_BINARY_DIR}/cmake_uninstall.cmake")
  endif(EXISTS "${CMAKE_UNINSTALL_TEMPLATE_FILE}")

endif(NOT TARGET uninstall)
