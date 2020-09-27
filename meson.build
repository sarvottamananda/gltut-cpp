project('opengl-snippets',
	['c', 'cpp'],
	version : '0.1.3',
	license : 'MIT',
	meson_version : '>= 0.49',
  	default_options : [
		'buildtype=release', 
		'c_std=c11', 
		'cpp_std=c++17', 
		'warning_level=3', 
		'layout=mirror'
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

glad_prj = subproject('glad')

glad_dep = glad_prj.get_variable('glad_dep')

subdir('src')
