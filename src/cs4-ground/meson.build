cs4_src = [ \
  'app.cpp', \
  'main.cpp', \
  'options.cpp', \
]

config_data = configuration_data()
config_data.set('CS_BUILD_DIR', meson.current_build_dir())
config_data.set('CS_SOURCE_DIR', meson.current_source_dir())

subdir('config')

subdir('assets')

cs4_exe = executable('cs4-ground', 
    cs4_src,  
    dependencies : [glfw_dep, yaml_dep, m_dep, glm_dep, glew_dep], 
    include_directories : [config_inc, cs_inc],
    link_with : cs_lib) 

test('cs4', cs4_exe, is_parallel : false, workdir : meson.current_build_dir())

