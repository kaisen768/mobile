# FindMxml
# -----------
#
# Find the Mxml library.
#
# Result Variables
# ^^^^^^^^^^^^^^^^
#
# This module will set the following variables in your project:
#
# ``MXML_FOUND``
#   System has the Mxml library.
# ``MXML_INCLUDE_DIR``
#   The Mxml include directory.
# ``MXML_LIBRARY``
#   The Mxml mxml library.
# ``MXML_LIBRARIES``
#   All Mxml libraries.
#
# Hints
# ^^^^^
#
# Set ``MXML_ROOT_DIR`` to the root directory of an Mxml installation.
# Set ``MXML_USE_STATIC_LIBS`` to ``TRUE`` to look for static libraries.

# Support preference of static libs by adjusting CMAKE_FIND_LIBRARY_SUFFIXES
if(MXML_USE_STATIC_LIBS)
    set(_mxml_ORING_CMAKE_FIND_LIBRARY_SUFFIXES ${CMAKE_FIND_LIBRARY_SUFFIXES})
    set(CMAKE_FIND_LIBRARY_SUFFIXES .a)
endif()

find_package(PkgConfig QUIET)
pkg_check_modules(MXML QUIET "mxml")

include(FindPackageHandleStandardArgs)
find_path(MXML_INCLUDE_DIR 
    NAMES 
        mxml.h
    )
find_library(MXML_LIBRARY 
    NAMES
        mxml
    )
find_package_handle_standard_args(MXML
    DEFAULT_MSG
    MXML_INCLUDE_DIR
    MXML_LIBRARY
    )

mark_as_advanced(
    MXML_INCLUDE_DIR
    MXML_LIBRARY
    )

if (MXML_FOUND)
    set(MXML_LIBRARIES ${MXML_LIBRARY})
    mark_as_advanced(MXML_LIBRARIES)
endif()

# Restore the original find library ordering
if(MXML_USE_STATIC_LIBS)
  set(CMAKE_FIND_LIBRARY_SUFFIXES ${_mxml_ORING_CMAKE_FIND_LIBRARY_SUFFIXES})
endif()
