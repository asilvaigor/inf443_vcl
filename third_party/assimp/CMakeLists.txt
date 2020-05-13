add_subdirectory(contrib/irrXML)
add_subdirectory(contrib/zlib)

include_directories(include)

file(GLOB_RECURSE assimp_files
        code/*.[ch]pp code/*.[ch]
        include/*.[ch]pp include/*.[ch])
add_library(assimp ${assimp_files})
target_link_libraries(assimp IrrXML zlib)