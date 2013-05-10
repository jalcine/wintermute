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

# Define the pre-release versions recognizable by Wintermute.
set(WINTERMUTE_VERSION_STAGE_NIGHTLY          0)
set(WINTERMUTE_VERSION_STAGE_ALPHA            1)
set(WINTERMUTE_VERSION_STAGE_BETA             2)
set(WINTERMUTE_VERSION_STAGE_RELEASECANDIDATE 3)
set(WINTERMUTE_VERSION_STAGE_FINALRELEASE     4)

## Load CMake modules necessary.
include(GetGitRevisionDescription)

##
## @fn grab_git_ref
## @param BRANCH   The branch to defining the reference.
## @param REF      The reference to be determined.
##
function(grab_git_ref)
  cmake_parse_arguments(ggr "" "BRANCH REF" "" ${ARGN})

  get_git_head_revision("${ggr_BRANCH}" "${ggr_REF}")
  
  message("br:${ggr_BRANCH}\nref:${ggr_REF}\n${ARGN}")
  string(SUBSTRING "${${ggr_REF}}" 0 8 "${ggr_REF}")
endfunction(grab_git_ref)
