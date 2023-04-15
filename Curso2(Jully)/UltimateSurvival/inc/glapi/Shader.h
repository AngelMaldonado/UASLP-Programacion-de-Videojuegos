#pragma once

#include <glapi/GLApi.h>

/**
 * Structure to store both vertex and fragment shader source codes as std::string
 */
struct ShaderSource {
	std::string vertexSource;
	std::string vertexSourcePath;
	std::string fragmentSource;
	std::string fragmentSourcePath;
};

class Shader
{
private:
	unsigned int idShaderProgram;
	ShaderSource shaderSource;
	// Mapa de las direcciones uniformes
	std::unordered_map<std::string, int> uniforms;

	void ProcessShaderFiles();
	unsigned int CompileShader(unsigned int shaderType, const std::string& shaderSource);
	unsigned int CreateShader();
	int GetUniformLocation(const std::string& uniformName);
public:
	Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath);
	~Shader();
	void Bind() const;
	void Unbind() const;
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform2fvec(const std::string& name, glm::vec2& value);
	void SetUniform3fvec(const std::string& name, glm::vec3& value);
	void SetUniform4fvec(const std::string& name, glm::vec4& value);
	void SetUniform2matf(const std::string& name, glm::mat2& value);
	void SetUniform3matf(const std::string& name, glm::mat3& value);
	void SetUniform4matf(const std::string& name, glm::mat4& value);
};

