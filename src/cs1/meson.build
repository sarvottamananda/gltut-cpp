cs1_sources = ['cs1_triangles.cpp']

cs1_exe = executable('cs1-triangles', cs1_sources, dependencies : [glfw_dep, glew_dep]) 

test('cs1', cs1_exe, is_parallel : false, workdir : meson.current_build_dir())

