# Tries to find the Qt Cryptography library (QCA).
#
# QCA_DEFINITIONS
# QCA_LIBRARIES
# QCA_INCLUDE_DIRS

find_package(PkgConfig)
pkg_check_modules(_QCA QUIET qca)
set(QCA_DEFINITIONS ${_QCA_CFLAGS_OTHER})

find_path(QCA_INCLUDE_DIR QtCrypto/qca.h
    HINTS ${_QCA_INCLUDEDIR} ${_QCA_INCLUDE_DIRS})

find_library(QCA_LIBRARY NAMES qca
    HINTS ${_QCA_LIBDIR} ${_QCA_LIBRARY_DIRS})

set(QCA_INCLUDE_DIRS ${QCA_INCLUDE_DIR})
set(QCA_LIBRARIES ${QCA_LIBRARY})

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QtCrypto DEFAULT_MSG
    QCA_LIBRARY QCA_INCLUDE_DIR)

mark_as_advanced(QCA_INCLUDE_DIR QCA_LIBRARY)
