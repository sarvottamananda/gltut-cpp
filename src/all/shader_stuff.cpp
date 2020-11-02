// Sarvottamananda (shreesh)
// 2020-09-28
// shader_stuff.cpp v0.0 (OpenGL Code Snippets)
//
// Low level shader stuff

// clang-format: off
#include <GL/glew.h>
// clang-format: on

// clang-format: off
#include "shader_stuff.h"
// clang-format: on

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>

using std::cerr;
using std::cout;
using std::string;
// template<class T> using Vector = std::vector<T, std::allocator<T>>;

GLuint create_shader(const string, const GLenum);

GLuint
create_program(string name, std::vector<string> shaders)
{
    cout << "Creating program (" << name << ")...\n";

    struct Shader_data {
	GLenum type;
	string ext;
	string name;
	bool present;
	GLuint id;
	string file;
    };

    std::vector<Shader_data> shdr_list = {
	Shader_data{GL_VERTEX_SHADER, "vert", "vertex", false, 0, ""},
	Shader_data{GL_TESS_CONTROL_SHADER, "tesc", "tesselation control", false, 0, ""},
	Shader_data{GL_TESS_EVALUATION_SHADER, "tese", "tesselation evaluation", false, 0, ""},
	Shader_data{GL_GEOMETRY_SHADER, "geom", "geometry", false, 0, ""},
	Shader_data{GL_FRAGMENT_SHADER, "frag", "fragment", false, 0, ""},
	Shader_data{GL_COMPUTE_SHADER, "comp", "compute", false, 0, ""},
    };

    //  Compile all the shaders given in parameter
    for (auto fn : shaders) {
	bool found_ext = false;

	string fn_ext = fn.substr(fn.find_last_of(".") + 1);
	for (auto &shdr : shdr_list) {
	    if (fn_ext == shdr.ext) {
		if (!shdr.present) {
		    found_ext = true;
		    shdr.present = true;
		    shdr.file = fn;
		    // cerr << "Creating shader ("<< fn << ")\n";
		    shdr.id = create_shader(fn, shdr.type);
		    break;
		}
		else {
		    std::stringstream msg;
		    msg << "Multiple shaders of same type! (" << fn << " & " << shdr.file
			<< ")";
		    throw std::runtime_error(msg.str());
		}
	    }
	}

	if (!found_ext) {
	    cerr << "Unknown shader! (" << fn << ")\n";
	}
    }
    // Vertex and fragment shaders are successfully compiled.

    // Get a program object.
    auto prog_id = glCreateProgram();

    // Attach all the compiled shaders above
    for (auto shdr : shdr_list) {
	if (shdr.present) {
	    // cerr << "Attaching shader ("<< shdr.file << ")\n";
	    glAttachShader(prog_id, shdr.id);
	}
    }

    // Link our program
    glLinkProgram(prog_id);

    // Check the linking status
    GLint isLinked = 0;
    glGetProgramiv(prog_id, GL_LINK_STATUS, (int *)&isLinked);
    if (isLinked == GL_FALSE) {
	GLint maxLength = 0;
	glGetProgramiv(prog_id, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	std::vector<GLchar> infoLog(maxLength);
	glGetProgramInfoLog(prog_id, maxLength, &maxLength, &infoLog[0]);

	// We don't need the program anymore.
	glDeleteProgram(prog_id);
	// Don't leak shaders either.

	for (auto shdr : shdr_list)
	    if (shdr.present) glDeleteShader(shdr.id);

	// We can make use of the infoLog if we want it, in the future .

	for (auto infoch : infoLog) {
	    cerr << infoch;
	}

	// In this simple program, we'll just throw
	std::stringstream msg;
	msg << "Linking failed (" << name << ")";
	throw std::runtime_error(msg.str());
    }
    cout << "Linking successful (" << name << ")\n\n";

    // Always detach shaders after a successful link.
    for (auto shdr : shdr_list)
	if (shdr.present) glDetachShader(prog_id, shdr.id);
    for (auto shdr : shdr_list)
	if (shdr.present) glDeleteShader(shdr.id);

    return prog_id;
}

static string read_file(std::string);

GLuint
create_shader(const string fn, const GLenum type)
// This function compiles the shader.
{
    string read_str = read_file(fn).c_str();  // Get source code for vertex shader.

    // Read the file.
    GLchar *shader_src = (GLchar *)read_str.c_str();

    // Compile the shader
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &shader_src, 0);
    glCompileShader(shader);

    // Check if compilation was okay
    GLint isCompiled = 0;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);

    if (isCompiled == GL_FALSE) {
	GLint maxLength = 512;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

	// The maxLength includes the NULL character
	std::string infoLog(maxLength, '\0');
	glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

	// We don't need the shader anymore.
	glDeleteShader(shader);

	// Use the infoLog as you see fit.
	std::cerr << infoLog;
	std::stringstream msg;
	msg << "Compilation failure (" << fn << ")";
	// In this simple program, we'll just throw
	throw std::runtime_error(msg.str());
    }
    std::cout << "Compilation successful (" << fn << ")";
    return shader;
}

static string
read_file(std::string filename)
{
    std::ifstream fs(filename);

    if (fs) {
	std::ostringstream ss;
	ss << fs.rdbuf();
	return ss.str();
    }
    else {
	std::stringstream msg;
	msg << "File does not exist (" << filename << ")";
	// In this simple program, we'll just throw
	throw std::runtime_error(msg.str());
    }
}
