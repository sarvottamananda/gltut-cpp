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

#cmake_policy(SET CMP0026 OLD)

set(ALL_LIBS
	${OPENGL_LIBRARY}
        ${YAML_CPP_LIBRARIES}
)

add_definitions(
    #-DGLEW_STATIC
        -std=c++20
)

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
    window_factory.cpp
    window_factory.h
    window_glfw.cpp
    window_glfw.h
    window.h
)

target_link_libraries(cs3
	${ALL_LIBS}
)
