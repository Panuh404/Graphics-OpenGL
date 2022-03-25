#pragma once

#include <string>
#include <unordered_map>

#include "glm.hpp"

enum shader_type {
	VERTEX_SHADER,
	FRAGMENT_SHADER
};

class Shader {
private:
	std::string m_VertexFilepath;
	std::string m_FragmentFilepath;
	unsigned int m_RendererID;
	mutable std::unordered_map<std::string, int> m_UniformLocationCache;
	
public:
	Shader(const std::string& VertexFilepath, const std::string& FragmentFilepath);
	~Shader();

	void Bind() const;
	void Unbind() const;
	
	// Uniforms
	void SetUniformli(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2f(const std::string& name, const glm::vec2& value);
	void SetUniform3f(const std::string& name, const glm::vec3& value);
	void SetUniform4f(const std::string& name, const glm::vec4& value);
	void SetUniformMat3(const std::string& name, const glm::mat3& matrix);
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix);

private:
	unsigned int CreateShader(const std::string& VertexShaderSource, const std::string& FragmentShaderSource);
	unsigned int CompileShader(const std::string& filepath, shader_type type);
	bool CheckShader(unsigned int shader, bool program = false);

	int GetUniformLocation(const std::string& name) const;
};