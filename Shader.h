#include <stdio.h>
#include <stdlib.h>
#include <glew.h>
#include <glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include "glm/ext.hpp"

class ShaderMesh {
public:
	unsigned int ID;
	void LoadShaders(std::string vertex_path, std::string fragment_path) {
		std::string vertex_code, fragment_code;
		std::ifstream vertex_file, fragment_file;
		vertex_file.exceptions(vertex_file.failbit | vertex_file.badbit);
		fragment_file.exceptions(fragment_file.failbit | fragment_file.badbit);
		try {
			std::stringstream vertex_stream, fragment_stream;
			vertex_file.open(vertex_path);
			fragment_file.open(fragment_path);
			vertex_stream << vertex_file.rdbuf();
			fragment_stream << fragment_file.rdbuf();
			vertex_file.close();
			fragment_file.close();
			vertex_code = vertex_stream.str();
			fragment_code = fragment_stream.str();
		}
		catch (const std::exception&) {
			std::cout << "Cann't open file\n";
		}
		const char* vertex_shader_code = vertex_code.c_str();
		const char* fragment_shader_code = fragment_code.c_str();
		unsigned int vertex, fragment;
		int success;
		char log[512];
		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vertex_shader_code, NULL);
		glCompileShader(vertex);
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, log);
			std::cout << "Compile Vertex Shader Error\n" << std::endl;
			return;
		}
		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fragment_shader_code, NULL);
		glCompileShader(fragment);
		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, log);
			std::cout << "Compile Fragment Shader Error\n" << std::endl;
			return;
		}
		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		glDetachShader(ID, vertex);
		glDetachShader(ID, fragment);
		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void set_uniform_bool(const std::string& name, bool value) {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}

	void set_uniform_int(const std::string& name, int value) {
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}

	void set_uniform_float(const std::string& name, float value) {
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}

	void set_vec3(const std::string& name, const glm::vec3& vec3) {
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &vec3[0]);
	}

	void set_mat4(const std::string& name, const glm::mat4& mat) {
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

	void use() {
		glUseProgram(ID);
	}
};