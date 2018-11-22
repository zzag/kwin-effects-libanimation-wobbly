# - Try to find libanimation
#
# Once done this will define
#  Libanimation_FOUND - System has libanimation
#  Libanimation_INCLUDE_DIRS - The libanimation include directories
#  Libanimation_LIBRARIES - The libraries needed to use libanimation
#
# Also, once done this will define the following targets
#  Libanimation::Libanimation
#

find_package (PkgConfig)
pkg_check_modules (PKG_Libanimation QUIET libanimation-0)

find_path (Libanimation_INCLUDE_DIR
    NAMES
        animation/wobbly/wobbly.h
    HINTS
        ${PKG_Libanimation_INCLUDE_DIRS}
)

find_library (Libanimation_LIBRARY
    NAMES
        animation
    HINTS
        ${PKG_Libanimation_LIBRARY_DIRS}
)

set (Libanimation_VERSION ${PKG_Libanimation_VERSION})
set (Libanimation_DEFINITIONS ${PKG_Libanimation_CFLAGS_OTHER})

include (FindPackageHandleStandardArgs)
find_package_handle_standard_args (Libanimation
    FOUND_VAR
        Libanimation_FOUND
    REQUIRED_VARS
        Libanimation_LIBRARY
        Libanimation_INCLUDE_DIR
    VERSION_VAR
        Libanimation_VERSION
)

if (Libanimation_FOUND)
    add_library (Libanimation::Libanimation UNKNOWN IMPORTED)
    set_target_properties (Libanimation::Libanimation PROPERTIES
        IMPORTED_LOCATION "${Libanimation_LIBRARY}"
        INTERFACE_COMPILE_OPTIONS "${Libanimation_DEFINITIONS}"
        INTERFACE_INCLUDE_DIRECTORIES "${Libanimation_INCLUDE_DIR}"
    )
endif ()

set (Libanimation_LIBRARARIES ${Libanimation_LIBRARY})
set (Libanimation_INCLUDE_DIRS ${Libanimation_INCLUDE_DIR})
set (Libanimation_VERSION_STRING ${Libanimation_VERSION})

mark_as_advanced (Libanimation_LIBRARY Libanimation_INCLUDE_DIR)
