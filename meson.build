project('opengl-cpp-tutorials',
	['c', 'cpp'],
	version : '0.1.4',
	license : 'MIT',
	meson_version : '>= 0.50',
  	default_options : [
		'buildtype=release', 
		'c_std=c18', 
		'cpp_std=c++2a', 
		'warning_level=2', 
		'layout=mirror',
                'default_library=static'
	]
)

c_compiler = meson.get_compiler('c')
cpp_compiler = meson.get_compiler('cpp')

add_project_arguments('-Wno-unused-parameter', language: ['c', 'cpp'])
add_project_arguments('-Wno-unused-function', language: ['c', 'cpp'])
add_project_arguments('-Wno-unused-result', language: ['c', 'cpp'])

dl_dep = c_compiler.find_library('dl', required : true)
glew_dep = dependency('glew')
glfw_dep = dependency('glfw3', version: '>= 3')
glm_dep = dependency('glm')
m_dep = cpp_compiler.find_library('m', required : false)
yaml_dep = dependency('yaml-cpp')

copy_to_build = find_program('scripts/copy_to_build.py')
copy = find_program('scripts/copy.py')

subdir('src')
