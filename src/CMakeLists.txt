file(GLOB_RECURSE src_files
        objects/*.cpp
        objects/*.h
        scene/*.cpp
        scene/*.h
        utils/*.cpp
        utils/*.h
        shaders/*.cpp
        shaders/*.h
        shaders/*.glsl
        )
add_library(src ${src_files})
target_link_libraries(src glfw)

add_executable(main main/main.cpp)
if (UNIX)
    target_link_libraries(main src vcl third_party glfw dl)
elseif (WIN32)
    target_link_libraries(main src vcl third_party ${GLFW_LIBRARIES})
endif ()