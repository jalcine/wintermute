# - Find Qt Cryptographic Architecture on the development system.
# This module finds if Qt Cryptographic Architecture is installed and determines where the
# include files and libraries are. It also determines what the name of
# the library is. This code sets the following variables:
#
#  QCA_LIBRARY                - path to the Qt Cryptographic Architecture library
#  QCA_LIBRARIES              - libraries for QCA
#  QCA_DEFINITIONS            - preferred compile definitions for QCA.
#  QCA_INCLUDE_DIR            - path to where qca.h is found.
#  QCA_INCLUDE_DIRS           - include paths for QCA.
#
#=============================================================================
# Copyright (c) 2012 Jacky Alcine <jacky.alcine@thesii.org>
#
# This module is free software; you can redistribute it and/or
# modify it under the terms of the GNU Library General Public
# License as published by the Free Software Foundation; either
# version 2 of the License, or (at your option) any later version.
#
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)

find_package(PkgConfig)
pkg_check_modules(_QCA QUIET qca)
set(QCA_DEFINITIONS ${_QCA_CFLAGS_OTHER})

find_path(QCA_INCLUDE_DIR QtCrypto/qca.h
    HINTS ${_QCA_INCLUDEDIR} ${_QCA_INCLUDE_DIRS})

find_library(QCA_LIBRARY NAMES qca
    HINTS ${_QCA_LIBDIR} ${_QCA_LIBRARY_DIRS})

set(QCA_INCLUDE_DIRS ${QCA_INCLUDE_DIR})
set(QCA_LIBRARIES ${QCA_LIBRARY})

list(APPEND QCA_INCLUDE_DIRS ${_QCA_INCLUDE_DIRS})
list(APPEND QCA_LIBRARIES ${_QCA_LIBRARIES})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QtCrypto DEFAULT_MSG
    QCA_LIBRARY QCA_INCLUDE_DIR)

mark_as_advanced(QCA_INCLUDE_DIR QCA_LIBRARY)
