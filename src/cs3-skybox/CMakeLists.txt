# CMake entry point
cmake_minimum_required (VERSION 3.0)
project (Code-snippets)

find_package(PkgConfig REQUIRED)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(yaml-cpp REQUIRED)

if (GLEW_FOUND)
    include_directories(${GLEW_INCLUDE_DIRS})
    link_libraries(${GLEW_LIBRARIES})
endif()

if (PKGCONFIG_FOUND)
    pkg_search_module(GLFW REQUIRED glfw3)
    include_directories(${GLFW_INCLUDE_DIRS})
    link_libraries(${GLFW_LIBRARIES})
endif()

#add_subdirectory (external)

#cmake_policy(SET CMP0026 OLD)

#list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-fe2273")
#include(CreateLaunchers)

#include_directories(
#   external/glfw-3.1.2/include/
#   external/glm-0.9.7.1/
#   external/glew-1.13.0/include/
#   external/assimp-3.0.1270/include/
#   .
#)

set(ALL_LIBS
	${OPENGL_LIBRARY}
        ${YAML_CPP_LIBRARIES}
)

add_definitions(
    #-DGLEW_STATIC
        -std=c++20
)

# Tutorial 2
add_executable(cs3 
    app_base.cpp
    app_base.h
    app.cpp
    app.h
    cs_config.h
    debug.cpp
    debug.h
    do_args.cpp
    do_args.h
    do_env.cpp
    do_env.h
    do_meson.cpp
    do_meson.h
    do_yaml.cpp
    do_yaml.h
    global.cpp
    global.h
    img_stuff.cpp
    img_stuff.h
    main.cpp
    model_base.cpp
    model_base.h
    model_cube.cpp
    model_cube.h
    options_store.cpp
    options_store.h
    shader_stuff.cpp
    shader_stuff.h
    stb_image_impl.cpp
    window.cpp
    window_glfw.cpp
    window_glfw.h
    window.h
)

target_link_libraries(cs3
	${ALL_LIBS}
)

#SOURCE_GROUP(common REGULAR_EXPRESSION ".*/common/.*" )
#SOURCE_GROUP(shaders REGULAR_EXPRESSION ".*/.*shader$" )

#add_custom_command(
#   TARGET tutorial02_red_triangle POST_BUILD
#   COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_INTDIR}/tutorial02_red_triangle${CMAKE_EXECUTABLE_SUFFIX}" "${CMAKE_CURRENT_SOURCE_DIR}/tutorial02_red_triangle/"
#)

