### MX Master control driver
## This utility allows to make some custom configurations to the MX Master
## mouse, monitor and react to its events (namely, forward/backward/thumb button
## press/release).
## Made by: haff

cmake_minimum_required(VERSION 3.16)
project(timestream)

### Project directories
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
set(SOURCES_DIR "${CMAKE_SOURCE_DIR}/src")

# Set compile parameters
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu11 -Wall -Wextra -pedantic")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g -DDEBUG")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -s -DNDEBUG -O3")

### Project directories
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")

## Installation paths if not specified
if (NOT DEFINED CMAKE_INSTALL_BINDIR)
  set (CMAKE_INSTALL_BINDIR "bin")
endif (NOT DEFINED CMAKE_INSTALL_BINDIR)

### Extract version numbers
include (git-version)
git_version_extract (TIMESTREAM)

add_subdirectory(${SOURCES_DIR})

### Packaging setup
include (InstallRequiredSystemLibraries)
set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Exact time streaming utility")
set (CPACK_PACKAGE_VENDOR "haff")
set (CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set (CPACK_PACKAGE_VERSION_MAJOR "${TIMESTREAM_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${TIMESTREAM_VERSION_MINOR}")
set (CPACK_SOURCE_GENERATOR "TGZ")
set (CPACK_SOURCE_PACKAGE_FILE_NAME
  "${CMAKE_PROJECT_NAME}-${TIMESTREAM_VERSION_FULL}")
set (CPACK_SOURCE_IGNORE_FILES
  ".*~"
  "#.*#"
  ".*.tar.gz"
  "/.git"
  "/_CPack_Packages"
  ".gitignore"
  "CMakeCache.txt"
  "CMakeFiles"
  "CPackConfig.cmake"
  "CPackSourceConfig.cmake"
  "Makefile"
  "cmake_install.cmake"
  "install_manifest.txt"
  )
include (CPack)
## Add alias to default 'dist' target
add_custom_target (dist COMMAND ${CMAKE_MAKE_PROGRAM} package_source)
