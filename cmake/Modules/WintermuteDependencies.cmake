#############################################################################
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
#############################################################################

if(_wntr_deps)
  return()
else()
  set(_wntr_deps TRUE)
endif()

# Include necessary modules.
include(FeatureSummary)

## Pass options to manipulate Wintermute's dependency.
option(PROVIDE_GUI_SUPPORT 
  "Compile Wintermute with QtGui libraries linked and used as run-time." ON)

## {{{ Packages
find_package(PkgConfig 0.24 REQUIRED)
find_package(Log4Qt REQUIRED)
find_package(QCommandLine 0.4.0 REQUIRED)
find_package(QJSON 0.7 REQUIRED)
find_package(QtZeroMQ REQUIRED)
find_package(Qt4 4.8 COMPONENTS 
  QtCore REQUIRED)
## }}}

## {{{ Determine which features and dependencies are flipped on/off.

add_feature_info("C++ Framework" QT_FOUND
  "The awesomest C++ framework on the planet.")
add_feature_info("Package Discovery" PKGCONFIG_FOUND
  "Allows for rapid package discovery.")
add_feature_info("JSON parsing" QJSON_FOUND
  "Allows for the use of JSON.")
add_feature_info("Command Line" QCOMMANDLINE_FOUND
  "Allows Wintermute to parse the command line.")
add_feature_info("Logging" Log4Qt_FOUND
  "Incorporates logging support.")

## }}}

## Provide feature report.
feature_summary(WHAT ALL)
