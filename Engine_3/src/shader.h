#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class shader
{
public:
	unsigned int id{};
	shader(const char* vertex_shader_path, const char* fragment_shader_path);
	void activate() const;

	// utility func
	std::string load_shader_src(const char* filepath);
	GLuint compile_shader(const char* filepath, GLenum type);

	// uniform func
	void set_mat4(const std::string& name, glm::mat4 value) const;

	void set_bool(const std::string& name, bool value) const;
	void set_int(const std::string& name, int value) const;
	void set_float(const std::string& name, float value) const;
	void set3_float(const std::string& name, float v1, float v2, float v3) const;
	void set3_float(const std::string& name, glm::vec3 v) const;
	void set4_float(const std::string& name, float v1, float v2, float v3, float v4) const;
	// void set4Float(const std::string& name, aiColor4D color);
	void set4_float(const std::string& name, glm::vec4 v) const;
	void set_mat3(const std::string& name, glm::mat3 val) const;
	void set_mat4(const std::string& name, glm::mat4 value);
};

#endif
