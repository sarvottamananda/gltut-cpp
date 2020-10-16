cs0_sources = ['cs0_window.cpp']

cs0_exe = executable('cs0-window', cs0_sources, dependencies : [glfw_dep, glew_dep] ) 

test('cs0', cs0_exe, is_parallel : false, workdir : meson.current_build_dir())

