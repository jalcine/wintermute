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

if (NOT DEFINED _wntr_mcrs)
	set(_wntr_mcrs ON)
else()
	return()
endif()

include(WintermuteVariables)
include(WintermuteDependencies)

#==============================================================================
#== @macro wintermute_add_target_properties
#== @param _target The target in question.
#== Takes a TARGET and applies Wintermute-specific properties to it. This
#== includes things that can vary on an environmental level as well.
#==============================================================================
MACRO(wintermute_add_target_properties _target)
	# Add inclusion directories to target.
	target_include_directories(${_target} BEFORE PUBLIC ${WINTERMUTE_INCLUDE_DIRS})

	# Add compilation flags for target.
	target_compile_options(${_target} BEFORE PUBLIC ${WINTERMUTE_COMPILE_FLAGS})

	# Add compilation definitions for target.
	target_compile_definitions(${_target} PUBLIC
		"${WINTERMUTE_COMPILE_DEFINITIONS}")

	if (CMAKE_BUILD_TYPE STREQUAL "Debug")
		target_compile_definitions(${_target} PRIVATE
			${WINTERMUTE_COMPILE_DEFINITIONS_DEBUG})
		target_compile_options(${_target} PRIVATE
			${WINTERMUTE_COMPILE_FLAGS_DEBUG})
	endif()
ENDMACRO(wintermute_add_target_properties)

#==============================================================================
#== @macro wintermute_link_libraries
#== @param _target The target in question.
#== Links TARGET with the libraries Wintermute would need in order to operate.
#== @note This typically isn't required unless you're interfacing with private
#== header information of Wintermute.
#==============================================================================
macro(wintermute_link_libraries _target)
  if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    if(CMAKE_COMPILER_IS_GNUCXX)
      set(WINTERMUTE_TEST_LIBRARIES gcov)
    elseif(CMAKE_CXX_COMPILER STREQUAL "Clang")
      set(WINTERMUTE_TEST_LIBRARIES profile_rt)
    endif()
  endif()

  target_link_libraries(${_target}
    ${WINTERMUTE_LIBRARIES}
    ${WINTERMUTE_TEST_LIBRARIES})
endmacro(wintermute_link_libraries _target)
