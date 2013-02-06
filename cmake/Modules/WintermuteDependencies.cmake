# Defines all of the dependencies Wintermute needs to be compiled and
# defines the paths and libraries for Wintermute building.
# Meant for internal use.

#=============================================================================
# Copyright (c) 2013 Jacky Alcine <jacky.alcine@thesii.org>
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

# Prevent re-loading.
if (DEFINED _wintermute_dependencies)
  return()
else(DEFINED _wintermute_dependencies)
  set(_wintermute_dependencies TRUE)
endif(DEFINED _wintermute_dependencies)

## VARIABLES
## =========
## = Define the minimal versions known to work with Wintermute here.
set(WINTERMUTE_QT_VERSION "4.7.2")
set(WINTERMUTE_QT_COMPONENTS "QtCore" "QtGui" "QtDBus")

## PACKAGE DISCOVERY
## =================
find_package(Qt4 ${WINTERMUTE_QT_VERSION} COMPONENTS ${WINTERMUTE_QT_COMPONENTS} REQUIRED)

## VARIABLES
## =========
## Define the inclusion directories for internal and external builds.
set(WINTERMUTE_INCLUDE_DIRS ${QT_HEADERS_DIR}
    ${QT_QTCORE_INCLUDE_DIR}
    ${QT_QTDBUS_INCLUDE_DIR})
set(WINTERMUTE_LIBRARIES ${QT_QTCORE_LIBRARY}
    ${QT_QTDBUS_LIBRARY})

if (WINTER_HEADLESS)
  list(REMOVE_ITEM WINTERMUTE_INCLUDE_DIRS ${QT_QTGUI_INCLUDE_DIR})
  list(REMOVE_ITEM WINTERMUTE_LIBRARIES    ${QT_QTGUI_LIBRARY})
else(WINTER_HEADLESS)
  list(APPEND WINTERMUTE_INCLUDE_DIRS      ${QT_QTGUI_INCLUDE_DIR})
  list(APPEND WINTERMUTE_LIBRARIES         ${QT_QTGUI_LIBRARY})
endif(WINTER_HEADLESS)
