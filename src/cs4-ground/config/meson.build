config_data.set('CS_CONFIG_FILE', 'cs4.yml')
config_data.set('CS_CONFIG_DIR', meson.current_source_dir())

configure_file(input : 'cs_config.h.in',
               output : 'cs_config.h',
               configuration : config_data)
config_inc = include_directories('.')

