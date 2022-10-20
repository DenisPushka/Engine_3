#include "shader.h"

shader::shader(const char* vertex_shader_path, const char* fragment_shader_path) {
	int success;

	const GLuint vertex_shader = compile_shader(vertex_shader_path, GL_VERTEX_SHADER);
	const GLuint fragment_shader = compile_shader(fragment_shader_path, GL_FRAGMENT_SHADER);

	id = glCreateProgram();
	glAttachShader(id, vertex_shader);
	glAttachShader(id, fragment_shader);
	glLinkProgram(id);

	// catch error
	glGetShaderiv(id, GL_LINK_STATUS, &success);
	if (!success) {
		char info_log[512];
		glGetShaderInfoLog(id, 512, nullptr, info_log);
		std::cout << "Linking error: " << std::endl << info_log << std::endl;
	}

	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

}

void shader::activate() const {
	glUseProgram(id);
}

std::string shader::load_shader_src(const char* filepath) {
	std::ifstream file;

	std::string ret;
	file.open(filepath);
	if (file.is_open()) {
		std::stringstream buf;
		buf << file.rdbuf();
		ret = buf.str();
	}
	else
		std::cout << "Could not open file" << filepath << std::endl;
	file.close();

	return ret;
}

GLuint shader::compile_shader(const char* filepath, const GLenum type) {
	int success;

	const GLuint ret = glCreateShader(type);
	const std::string shader_src = load_shader_src(filepath);
	const GLchar* shader = shader_src.c_str();
	glShaderSource(ret, 1, &shader, nullptr);
	glCompileShader(ret);

	// catch error
	glGetProgramiv(ret, GL_LINK_STATUS, &success);
	if (!success) {
		char info_log[512];
		glGetProgramInfoLog(ret, 512, nullptr, info_log);
		std::cout << "Error with shader_program compile: " << std::endl << info_log << std::endl;
	}

	return ret;
}

void shader::set_mat4(const std::string& name, glm::mat4 value) const {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}

void shader::set_bool(const std::string& name, const bool value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), static_cast<int>(value));
}

void shader::set_int(const std::string& name, const int value) const {
	glUniform1i(glGetUniformLocation(id, name.c_str()), value);
}

void shader::set_float(const std::string& name, const float value) const {
	glUniform1f(glGetUniformLocation(id, name.c_str()), value);
}

void shader::set3_float(const std::string& name, float v1, float v2, float v3) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), v1, v2, v3);
}

void shader::set3_float(const std::string& name, glm::vec3 v) const {
	glUniform3f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z);
}

void shader::set4_float(const std::string& name, float v1, float v2, float v3, float v4) const {
	glUniform4f(glGetUniformLocation(id, name.c_str()), v1, v2, v3, v4);
}

//void shader::set4Float(const std::string& name, aiColor4D color) {
//	glUniform4f(glGetUniformLocation(id, name.c_str()), color.r, color.g, color.b, color.a);
//}

void shader::set4_float(const std::string& name, glm::vec4 v) const {
	glUniform4f(glGetUniformLocation(id, name.c_str()), v.x, v.y, v.z, v.w);
}

void shader::set_mat3(const std::string& name, glm::mat3 val) const {
	glUniformMatrix3fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(val));
}

void shader::set_mat4(const std::string& name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}
