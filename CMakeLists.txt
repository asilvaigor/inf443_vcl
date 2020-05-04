cmake_minimum_required(VERSION 2.8)
project(pgm)

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

# Add G++ Warning on Unix
if (UNIX)
    add_definitions(-g -O3 -std=c++11 -Wall -Wextra)
    set(CMAKE_CXX_COMPILER g++)
    find_package(glfw3 REQUIRED) #Expect glfw3 to be installed on your system
# In Window set directory to precompiled version of glfw3
elseif (WIN32)
    set(CMAKE_BUILD_TYPE Debug) # Switch to Release for faster execution
    set(GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/lib_windows/glfw3_win/include")
    include_directories(${GLFW_INCLUDE_DIRS})
    set(GLFW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib_windows/glfw3_win/lib/glfw3.lib")
endif ()

# Creating bin folder
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

include_directories(".")
include_directories("third_party/eigen/")
include_directories(vcl)
include_directories(src)

file(GLOB_RECURSE vcl_files vcl/*.[ch]pp)
add_library(vcl ${vcl_files})
target_link_libraries(vcl glfw)

file(GLOB_RECURSE third_party_files third_party/*.[ch]pp third_party/*.[ch])
add_library(third_party ${third_party_files})
target_link_libraries(third_party glfw)

add_subdirectory(src)
add_subdirectory(tests)
