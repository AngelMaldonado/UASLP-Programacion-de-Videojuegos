#include <glapi/GLApi.h>

Shader::Shader(const std::string& vertexSourcePath, const std::string& fragmentSourcePath)
{
	shaderSource.vertexSourcePath = vertexSourcePath;
	shaderSource.fragmentSourcePath = fragmentSourcePath;
	ProcessShaderFiles();
	idShaderProgram = CreateShader();
}

Shader::~Shader()
{
	GLCall(glDeleteProgram(idShaderProgram));
}

void Shader::Bind() const
{
	GLCall(glUseProgram(idShaderProgram));
}

void Shader::Unbind() const
{
    GLCall(glUseProgram(0));
}

int Shader::GetUniformLocation(const std::string& uniformName)
{
	if (uniforms.find(uniformName) != uniforms.end()) return uniforms[uniformName];
	GLCall(int uniformDir = glGetUniformLocation(idShaderProgram, uniformName.c_str()));
	if (uniformDir == -1) std::cout << "Uniform variable not found: " << uniformName << std::endl;
	uniforms[uniformName] = uniformDir;
	return uniformDir;
}
	
void Shader::SetUniform1i(const std::string& name, int value)
{
	GLCall(glUniform1i(GetUniformLocation(name), value));
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform2fvec(const std::string& name, glm::vec2& value)
{
	GLCall(glUniform2fv(GetUniformLocation(name), 1, &value[0]));
}

void Shader::SetUniform3fvec(const std::string& name, glm::vec3& value)
{
	GLCall(glUniform3fv(GetUniformLocation(name), 1, &value[0]));
}

void Shader::SetUniform4fvec(const std::string& name, glm::vec4& value)
{
	GLCall(glUniform3fv(GetUniformLocation(name), 1, &value[0]));
}

void Shader::SetUniform2matf(const std::string& name, glm::mat2& value)
{
	GLCall(glUniformMatrix2fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]));
}

void Shader::SetUniform3matf(const std::string& name, glm::mat3& value)
{
	GLCall(glUniformMatrix3fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]));
}

void Shader::SetUniform4matf(const std::string& name, glm::mat4& value)
{
	GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &value[0][0]));
}

void Shader::ProcessShaderFiles()
{
	std::ifstream vertexReader;
	std::ifstream fragmentReader;
	std::stringstream vertexStream;
	std::stringstream fragmentStream;

	vertexReader.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragmentReader.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		vertexReader.open(shaderSource.vertexSourcePath);
		fragmentReader.open(shaderSource.fragmentSourcePath);

		vertexStream << vertexReader.rdbuf();
		fragmentStream << fragmentReader.rdbuf();

		vertexReader.close();
		fragmentReader.close();

		shaderSource.vertexSource = vertexStream.str();
		shaderSource.fragmentSource = fragmentStream.str();
	}
	catch (std::ifstream::failure& error) { std::cout << "Error reading shader: " << error.what() << std::endl; }
}

unsigned int Shader::CreateShader()
{
	unsigned int mainShaderProgram = glCreateProgram();
	unsigned int vertexShaderProgram = CompileShader(GL_VERTEX_SHADER, shaderSource.vertexSource);
	unsigned int fragmentShaderProgram = CompileShader(GL_FRAGMENT_SHADER, shaderSource.fragmentSource);

	GLCall(glAttachShader(mainShaderProgram, vertexShaderProgram));
	GLCall(glAttachShader(mainShaderProgram, fragmentShaderProgram));
	GLCall(glLinkProgram(mainShaderProgram));
	GLCall(glDeleteShader(vertexShaderProgram));
	GLCall(glDeleteShader(fragmentShaderProgram));

	return mainShaderProgram;
}

unsigned int Shader::CompileShader(unsigned int shaderType, const std::string& shaderSource)
{
	const char* source = shaderSource.c_str();
	char* log;
	int compileResult, errorLogLength;
	GLCall(unsigned int id = glCreateShader(shaderType));
	GLCall(glShaderSource(id, 1, &source, nullptr));
	GLCall(glCompileShader(id));
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &compileResult));

	if (compileResult == GL_FALSE)
	{
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLogLength));
		log = (char*)_malloca(errorLogLength * sizeof(char));
		GLCall(glGetShaderInfoLog(id, errorLogLength, &errorLogLength, log));
		std::cout << "Shader compile error [" << (shaderType == GL_VERTEX_SHADER ? "vertex]" : "fragment]");
		std::cout << ": " << log << std::endl;
		GLCall(glDeleteShader(id));
		return 0;
	}
	return id;
}

