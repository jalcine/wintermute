###############################################################################
### Copyright (C) 2013 Jacky Alcine <me@jalcine.me>
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

if(_wntr_deps)
  return()
else()
  set(_wntr_deps TRUE)
endif()

# Include necessary modules.
include(FeatureSummary)
include(FindPkgConfig)

## Pass options to manipulate Wintermute's dependency.
option(PROVIDE_GUI_SUPPORT "Compile Wintermute with QtGui libraries linked and used as run-time." ON)

## Define variables necessary for Qt discovery.
set(WINTERMUTE_QT_VERSION 4.7)

## {{{ Look for Qt, if not found already.

if (NOT DEFINED QT_FOUND OR NOT ${WINTERMUTE_QT_VERSION} EQUAL "${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")
  find_package(Qt4 ${WINTERMUTE_QT_VERSION} COMPONENTS
    QtCore
    REQUIRED)
endif()

## }}}

## {{{ Packages

find_package(Log4Qt REQUIRED)
find_package(QCommandLine REQUIRED)

pkg_check_modules(ZMQ REQUIRED libzmq)

## }}}

## {{{ Determine which features and dependencies are flipped on/off.

add_feature_info("GUI" QT_QTGUI_FOUND "Allows for the rendering of graphical elements.")
add_feature_info("Command Line" QCOMMANDLINE_FOUND
  "Allows Wintermute to parse the command line.")
add_feature_info("Logging" Log4Qt_FOUND "Incorporates logging support.")
add_feature_info("Procedure Calling" ZMQ_FOUND "Incorporates a transparent means of communicating inbetween processes.")

## }}}

## Provide feature report.
feature_summary(WHAT ALL)
