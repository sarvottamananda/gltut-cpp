project('opengl-cpp-tutorials',
	['c', 'cpp'],
	version : '0.6',
	license : 'MIT',
	meson_version : '>= 0.56',
  	default_options : [
		'buildtype=release', 
		'c_std=c11', 
		'cpp_std=c++2a', 
		'warning_level=3', 
		'layout=mirror',
                'default_library=static'
	]
)

c_compiler = meson.get_compiler('c')
cpp_compiler = meson.get_compiler('cpp')

#add_project_arguments('-Wall', language: ['c', 'cpp'])
#add_project_arguments('-Wextra', language: ['c', 'cpp'])
#add_project_arguments('-Wpedantic', language: ['c', 'cpp'])
#add_project_arguments('-Wshadow', language: ['c', 'cpp'])

add_project_arguments('-Wnon-virtual-dtor', language: ['c', 'cpp'])
add_project_arguments('-Wno-unused-parameter', language: ['c', 'cpp'])
add_project_arguments('-Wno-unused-function', language: ['c', 'cpp'])
add_project_arguments('-Wno-unused-result', language: ['c', 'cpp'])

cs_inc = include_directories('src/inc')

#dl_dep = c_compiler.find_library('dl', required : true)
glew_dep = dependency('glew')
glfw_dep = dependency('glfw3', version: '>= 3')
glm_dep = dependency('glm')
m_dep = cpp_compiler.find_library('m', required : false)
#yaml_dep = dependency('yaml-cpp')

copy_to_build = find_program('scripts/copy_to_build.py')
copy = find_program('scripts/copy.py')

subdir('src/all')
subdir('src/cs0')
subdir('src/cs1')
subdir('src/cs2')
subdir('src/cs3')
subdir('src/cs4')
subdir('src/cs5')
subdir('src/cs6')
