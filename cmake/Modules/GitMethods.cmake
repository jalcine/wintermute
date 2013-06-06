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

find_package(Git 1.7 REQUIRED)
include(CMakeParseArguments)

function(git_invoke)
  set(_oneArgs ARGS RESULT)
  cmake_parse_arguments(GIT_INVOKE "" "${_oneArgs}" "" ${ARGN})
  execute_process(COMMAND ${GIT_EXECUTABLE} "${GIT_INVOKE_ARGS}"
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_VARIABLE _ref)
  message(STATUS ${_ref})
endfunction(git_invoke)

function(git_current_branch _branch)
  git_invoke(ARGS "rev-parse --abbrev-ref HEAD" RESULT ${_branch})
endfunction(git_current_branch)

function(git_current_ref _ref)
  set(a_branch )
  git_current_branch(a_branch)
  git_grab_ref(BRANCH "${${_branch}}" REF _ref)
endfunction(git_current_ref)

function(git_grab_ref)
  cmake_parse_arguments(ggr "" "BRANCH REF" "" ${ARGN})
  git_invoke(ARGS "rev-parse ${ggr_BRANCH}" RESULT "${ggr_REF}")
endfunction(git_grab_ref)  
