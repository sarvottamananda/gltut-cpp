cs5_src = [ \
  'app_lighting.cpp', \
  'cs5_lighting.cpp', \
]

cs5_exe = executable('cs5-lighting', 
    cs5_src,  
    dependencies : [glfw_dep, m_dep, glm_dep, glew_dep], 
    include_directories :  cs_inc,
    link_with : cs_lib) 

test('cs5', cs5_exe, is_parallel : false, workdir : meson.current_build_dir())

config_data = configuration_data()
config_data.set('CS_BUILD_DIR', meson.current_build_dir())
config_data.set('CS_SOURCE_DIR', meson.current_source_dir())
config_data.set('CS_ROOT_DIR', meson.project_source_root())

configure_file(input : 'cs5_config.h.in',
               output : 'cs5_config.h',
               configuration : config_data)
config_inc = include_directories('.')

run_command('ctags', '-R', '--languages=C,C++', '--kinds-C++=+p', \
    '--fields=+iaS', '--extras=+qFr', '--sort=yes')

