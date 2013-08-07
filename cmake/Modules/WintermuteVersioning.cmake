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

include(GitMethods)

# Define the pre-release versions recognizable by Wintermute.
set(WINTERMUTE_VERSION_STAGE_NIGHTLY          0)
set(WINTERMUTE_VERSION_STAGE_ALPHA            1)
set(WINTERMUTE_VERSION_STAGE_BETA             2)
set(WINTERMUTE_VERSION_STAGE_RELEASECANDIDATE 3)
set(WINTERMUTE_VERSION_STAGE_FINALRELEASE     4)

## Define Wintermute's current version.
set(WINTERMUTE_VERSION_MAJOR  0)
set(WINTERMUTE_VERSION_MINOR  0)
set(WINTERMUTE_VERSION_PATCH  1)
set(WINTERMUTE_VERSION_STAGE  ${WINTERMUTE_VERSION_STAGE_NIGHTLY})
set(WINTERMUTE_VERSION        "${WINTERMUTE_VERSION_MAJOR}.${WINTERMUTE_VERSION_MINOR}.${WINTERMUTE_VERSION_PATCH}")

## Obtain versioning information.
if (PROJECT_NAME STREQUAL "Wintermute")
  git_current_branch(WINTERMUTE_VERSION_STAGE_BRANCH)
  git_current_ref(WINTERMUTE_VERSION_STAGE_REF)
  message(STATUS "[Wintermute] Current ref and branch: ${WINTERMUTE_VERSION_STAGE_BRANCH} (${WINTERMUTE_VERSION_STAGE_REF})")
endif()

