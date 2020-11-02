cs4_src = [ \
  'app_ground.cpp', \
  'cs4_ground.cpp', \
]

cs4_exe = executable('cs4-ground', 
    cs4_src,  
    dependencies : [glfw_dep, m_dep, glm_dep, glew_dep], 
    include_directories :  cs_inc,
    link_with : cs_lib) 

test('cs4', cs4_exe, is_parallel : false, workdir : meson.current_build_dir())

config_data = configuration_data()
config_data.set('CS_BUILD_DIR', meson.current_build_dir())
config_data.set('CS_SOURCE_DIR', meson.current_source_dir())
config_data.set('CS_ROOT_DIR', meson.project_source_root())

configure_file(input : 'cs4_config.h.in',
               output : 'cs4_config.h',
               configuration : config_data)
config_inc = include_directories('.')

run_command('ctags', '-R', '--languages=C,C++', '--kinds-C++=+p', \
    '--fields=+iaS', '--extras=+qFr', '--sort=yes')

