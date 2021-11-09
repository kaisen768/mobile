cmake_minimum_required(VERSION 3.4)
cmake_policy(SET CMP0048 NEW)

mark_as_advanced(MOBILE_VERSION)
set(MOBILE_VERSION 1.0.0)

project(
  mobile
  VERSION ${MOBILE_VERSION}
  LANGUAGES C)

add_compile_options(-std=gnu11)

# configure
######################################################################
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

option(MOBILE_UV_STATIC "Uv use static library" OFF)
option(MOBILE_MXML_STATIC "Mxml use static library" OFF)
option(MOBILE_USE_JEMALLOC "Use Jemalloc" OFF)

include(CMakeDependentOption)
include(GNUInstallDirs)

if(MOBILE_UV_STATIC)
  set(UV_USE_STATIC_LIBS TRUE)
endif()
find_package(uv REQUIRED)

if(MOBILE_MXML_STATIC)
  set(MXML_USE_STATIC_LIBS TRUE)
endif()
find_package(Mxml REQUIRED)

if(MOBILE_USE_JEMALLOC)
  find_package(Jemalloc)
endif()

# header
######################################################################
mark_as_advanced(mobile_include_dirs)

list(APPEND mobile_include_dirs ${UV_INCLUDE_DIR})
list(APPEND mobile_include_dirs ${MXML_INCLUDE_DIR})

if(MOBILE_USE_JEMALLOC)
  list(APPEND mobile_include_dirs ${JEMALLOC_INCLUDE_DIR})
endif(MOBILE_USE_JEMALLOC)

include_directories(${mobile_include_dirs})

# source file
######################################################################
mark_as_advanced(mobile_srcs)

aux_source_directory(${mobile_SOURCE_DIR}/src mobile_srcs)

# package
######################################################################
include(CMakePackageConfigHelpers)

configure_package_config_file(
  "${mobile_SOURCE_DIR}/Config.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/generated/MobileConfig.cmake"
  INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Mobile")

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/generated/MobileConfigVersion.cmake"
    VERSION ${MOBILE_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/generated/MobileConfig.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/generated/MobileConfigVersion.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Mobile")

configure_file("${mobile_SOURCE_DIR}/mobile.pc.in"
              "${CMAKE_CURRENT_BINARY_DIR}/generated/mobile.pc" @ONLY)

install(FILES "${CMAKE_CURRENT_BINARY_DIR}/generated/mobile.pc"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig")

# library generate
######################################################################
function(mobile_library_generator name type target_name)
  add_library(${name} ${type} ${mobile_srcs})
  set_target_properties(${name} PROPERTIES OUTPUT_NAME "mobile"
                                           CLEAN_DIRECT_OUTPUT 1)

  mark_as_advanced(__mobile_opensource_libraries)
  list(APPEND __mobile_opensource_libraries ${UV_LIBRARY})
  list(APPEND __mobile_opensource_libraries ${MXML_LIBRARY})
  if(MOBILE_USE_JEMALLOC)
    list(APPEND __mobile_opensource_libraries ${JEMALLOC_LIBRARY})
  endif()
  target_link_libraries(${name} PUBLIC ${__mobile_opensource_libraries})

  if(type STREQUAL "SHARED")
    set_target_properties(
      ${name}
      PROPERTIES VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
                 SOVERSION ${PROJECT_VERSION_MAJOR})
  endif()

  set_target_properties(
    ${name} PROPERTIES COMPILE_FLAGS "${CMAKE_C_FLAGS} ${CMAKE_CXX_FLAGS}")

  install(
    TARGETS ${name}
    EXPORT ${target_name}
    RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}"
    LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}")

  install(EXPORT ${target_name}
          DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/Mobile")
endfunction()

mobile_library_generator(mobile SHARED MobileTarget)
mobile_library_generator(mobile-static STATIC MobileStaticTarget)

# add subdirectory
######################################################################
mark_as_advanced(__mobile_include_dir)
foreach(__mobile_include_dir ${mobile_include_dirs})
  target_include_directories(
    mobile-static SYSTEM
    INTERFACE
      "$<BUILD_INTERFACE:${__mobile_include_dir}>"
      "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
endforeach()

target_include_directories(
  mobile-static SYSTEM
  INTERFACE
    "$<BUILD_INTERFACE:${UV_INCLUDE_DIR}>"
    "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

target_include_directories(
  mobile-static SYSTEM
    INTERFACE
      "$<BUILD_INTERFACE:${MXML_INCLUDE_DIR}>"
      "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

if(MOBILE_USE_JEMALLOC)
  target_include_directories(
    mobile-static SYSTEM
    INTERFACE
      "$<BUILD_INTERFACE:${JEMALLOC_INCLUDE_DIR}>"
      "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")
endif()
