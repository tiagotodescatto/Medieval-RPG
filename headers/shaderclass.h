#ifndef  SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader {
public:
	unsigned int ID;
	Shader(const char* vertexPath, const char* fragPath) {
		std::string vertexCode, fragCode;
		std::ifstream vShaderFile, fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try {
			vShaderFile.open(vertexPath);
			fShaderFile.open(fragPath);
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragCode = fShaderStream.str();
		}
		catch (std::ifstream::failure e) {
			std::cerr << "FAILED::TO::READ::SHADER::FILE\n";
		}

		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragCode.c_str();

		unsigned int vertex, fragment;
		int success;
		char infoLog[512];

		vertex = glCreateShader(GL_VERTEX_SHADER);
		fragment = glCreateShader(GL_FRAGMENT_SHADER);

		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);

		glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(vertex, 512, NULL, infoLog);
			std::cerr << "ERROR::FAILED::TO::COMPILE::VERTEX::SHADERS\N" << infoLog << std::endl;
		}

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
		if (!success) {
			glGetShaderInfoLog(fragment, 512, NULL, infoLog);
			std::cerr << "ERROR::FAILED::TO::COMPILE::FRAGMENT::SHADERS\N" << infoLog << std::endl;
		}

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);

		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(ID, 512, NULL, infoLog);
			std::cerr << "ERROR::FAILED::TO::LINK::SHADER::PROGRAM\N" << infoLog << std::endl;
		}

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	void use() {
		glUseProgram(ID);
	}

	void setInt(const std::string& name, int value)
	{
		glUniform1i(
			glGetUniformLocation(ID, name.c_str()),
			value
		);
	}

	void setMat4(const std::string& name, const glm::mat4& value)
	{
		glUniformMatrix4fv(
			glGetUniformLocation(ID, name.c_str()),
			1,
			GL_FALSE,
			glm::value_ptr(value)
		);
	}

	void setVec3(const std::string& name, const glm::vec3& value)
	{
		glUniform3fv(
			glGetUniformLocation(ID, name.c_str()),
			1,
			glm::value_ptr(value)
		);
	}
};

#endif
