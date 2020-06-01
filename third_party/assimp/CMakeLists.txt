# Search for zlib
find_package(ZLIB)

if(NOT ZLIB_FOUND)
  message(STATUS "compiling zlib from sources")
  include(CheckIncludeFile)
  include(CheckTypeSize)
  include(CheckFunctionExists)

  # Explicitly turn off ASM686 and AMD64 cmake options.
  # The AMD64 option causes a build failure on MSVC and the ASM builds seem to have problems:
  #		https://github.com/madler/zlib/issues/41#issuecomment-125848075
  # Also prevents these options from "polluting" the cmake options if assimp is being
  # included as a submodule.
  set(ASM686 FALSE CACHE INTERNAL "Override ZLIB flag to turn off assembly" FORCE)
  set(AMD64 FALSE CACHE INTERNAL "Override ZLIB flag to turn off assembly" FORCE)

  # compile from sources
  add_subdirectory(contrib/zlib)
  set(ZLIB_FOUND 1)
  set(ZLIB_LIBRARIES zlibstatic)
  set(ZLIB_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/contrib/zlib ${CMAKE_CURRENT_BINARY_DIR}/contrib/zlib)
  # need to ensure we don't link with system zlib or minizip as well.
else()
  add_definitions(-DASSIMP_BUILD_NO_OWN_ZLIB)
  set(ZLIB_LIBRARIES_LINKED -lz)
#  use_pkgconfig(UNZIP minizip)
endif()
include_directories(${ZLIB_INCLUDE_DIR})


add_subdirectory(contrib/irrXML)

include_directories(include)

file(GLOB_RECURSE assimp_files
        code/*.[ch]pp code/*.[ch]
        include/*.[ch]pp include/*.[ch])
add_library(assimp ${assimp_files})
target_link_libraries(assimp IrrXML z)
