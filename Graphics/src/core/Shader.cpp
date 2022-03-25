#include <iostream>
#include <fstream>

#include "Shader.h"
#include "Renderer.h"

Shader::Shader(const std::string& VertexFilepath, const std::string& FragmentFilepath) :
	m_VertexFilepath(VertexFilepath),
	m_FragmentFilepath(FragmentFilepath),
	m_RendererID(0)
{
	const std::string& VertexShaderSource = VertexFilepath;
	const std::string& FragmentShaderSource = FragmentFilepath;
	m_RendererID = CreateShader(VertexShaderSource, FragmentShaderSource);
}

Shader::~Shader() {
	glDeleteProgram(m_RendererID);
}

unsigned int Shader::CompileShader(const std::string& filepath, shader_type type) {
	std::ifstream ShaderStream(filepath);
	std::string shaderSource;
	std::string Line;
	
	while (std::getline(ShaderStream, Line)) {
		shaderSource += "\n" + Line;
	}
	
	unsigned int shader;
	const char* src = shaderSource.c_str();

	if (type == VERTEX_SHADER) {
		shader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
		if (!CheckShader(shader)) {
			glDeleteShader(shader);
			return 0;
		}
		return shader;

	}
	else if (type == FRAGMENT_SHADER) {
		shader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(shader, 1, &src, nullptr);
		glCompileShader(shader);
		if (!CheckShader(shader)) {
			glDeleteShader(shader);
			return 0;
		}
		return shader;
	}
}

unsigned int Shader::CreateShader(const std::string& VertexShaderSource, const std::string& FragmentShaderSource) {
	unsigned int VertexShader =		CompileShader(VertexShaderSource,	VERTEX_SHADER);
	unsigned int FragmentShader =	CompileShader(FragmentShaderSource,	FRAGMENT_SHADER);
	unsigned int ShaderProgram = glCreateProgram();
	
	glAttachShader(ShaderProgram, VertexShader);
	glAttachShader(ShaderProgram, FragmentShader);
	
	// Links and validate Shader Program
	glLinkProgram(ShaderProgram);
	glValidateProgram(ShaderProgram);

	// Check and Delete Shader Objects
	if (!CheckShader(ShaderProgram, true)) {
		glDeleteProgram(ShaderProgram);
		return 0;
	}
	glDeleteShader(VertexShader);
	glDeleteShader(FragmentShader);
	
	return ShaderProgram;
}


bool Shader::CheckShader(unsigned int shader, bool program) {
	int success;
	char infoLog[512];
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

	if (!success and !program) {
		glGetShaderInfoLog(shader, 512, NULL, infoLog);
		std::cout <<  "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	else if (!success and program) {
		glGetProgramInfoLog(shader, 512, NULL, infoLog);
		std::cout << "ERROR::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	}
	if (success and !program) {
		std::cout << "SHADER::COMPILATION_SUCCESSFUL" << std::endl;
		return true;
	}
	else if (success and program) {
		std::cout << "PROGRAM::COMPILATION_SUCCESSFUL" << std::endl;
		return true;
	}
}

void Shader::Bind() const {
	glUseProgram(m_RendererID);
}
void Shader::Unbind() const {
	glUseProgram(0);
}

void Shader::SetUniformli(const std::string& name, int value) {}
void Shader::SetUniform1f(const std::string& name, float value) {}
void Shader::SetUniform2f(const std::string& name, const glm::vec2& value) {}
void Shader::SetUniform3f(const std::string& name, const glm::vec3& value) {}
void Shader::SetUniform4f(const std::string& name, const glm::vec4& value) {}
void Shader::SetUniformMat3(const std::string& name, const glm::mat3& matrix) {}
void Shader::SetUniformMat4(const std::string& name, const glm::mat4& matrix) {}