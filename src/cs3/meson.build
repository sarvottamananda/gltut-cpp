cs3_src = [ \
  'app_base.cpp', \
  'app_skybox.cpp', \
  'cs3_skybox.cpp', \
  'debug.cpp', \
  'do_args.cpp', \
  'do_meson.cpp', \
  'img_stuff.cpp', \
  'model_base.cpp', \
  'model_cube.cpp', \
  'options_store.cpp', \
  'shader_stuff.cpp', \
  'stb_image_impl.cpp', \
  'window.cpp', \
  'window_factory.cpp', \
  'window_glfw.cpp', \
]

config_data = configuration_data()
config_data.set('CS_BUILD_DIR', meson.current_build_dir())
config_data.set('CS_SOURCE_DIR', meson.current_source_dir())
config_data.set('CS_ROOT_DIR', meson.project_source_root())

configure_file(input : 'cs3_config.h.in',
               output : 'cs3_config.h',
               configuration : config_data)
config_inc = include_directories('.')

cs3_exe = executable('cs3-skybox', 
    cs3_src,  
    dependencies : [glfw_dep, m_dep, glm_dep, glew_dep], 
    include_directories : [config_inc, cs_inc]) 

test('cs3', cs3_exe, is_parallel : false, workdir : meson.current_build_dir())
