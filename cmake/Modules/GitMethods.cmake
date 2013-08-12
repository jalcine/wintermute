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

##
## @fn git_invoke
## @brief Invokes command via the Git command.
##
function(git_invoke)
  set(_oneArgs   RESULT)
  set(_multiArgs ARGS)
  cmake_parse_arguments(gi "" "RESULT" "ARGS" ${ARGN})

  execute_process(COMMAND ${GIT_EXECUTABLE} ${gi_ARGS}
    OUTPUT_VARIABLE   _ref
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    OUTPUT_STRIP_TRAILING_WHITESPACE)

  set(${gi_RESULT} ${_ref} CACHE STRING "Git information.")
endfunction(git_invoke)

function(git_current_branch _branch)
  git_invoke(ARGS rev-parse HEAD RESULT ${_branch})
endfunction(git_current_branch)

function(git_current_ref _ref)
  git_invoke(ARGS rev-parse HEAD RESULT ${_ref})
endfunction(git_current_ref)
