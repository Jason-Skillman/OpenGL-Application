#pragma once

#include <string>
#include <unordered_map>

#include "glm/fwd.hpp"

class ShaderProgramSource;

class Shader {
private:
	std::string filePath;
	unsigned int rendererId;
	mutable std::unordered_map<std::string, int> uniformLocationCache;
	
public:
	Shader(const std::string& filePath);
	~Shader();
	
public:
	void Bind() const;
	void Unbind() const;

	//Set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
	void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);

private:
	ShaderProgramSource ParseShader(const std::string& filePath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	
	int GetUniformLocation(const std::string& name) const;
	
};

