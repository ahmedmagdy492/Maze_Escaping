#include "../include/Shader.h"

#include <sstream>
#include <fstream>

#include <glad/glad.h>

static std::stringstream ReadFile(const std::string& filePath) {
	std::ifstream ifstream(filePath.c_str());
	std::stringstream strStream;

	std::string currentLine;
	while (std::getline(ifstream, currentLine)) {
		strStream << currentLine << std::endl;
	}

	ifstream.close();

	return strStream;
}

static bool CompileShader(unsigned int shaderId) {
	glCompileShader(shaderId);

	int success;
	char infoLog[512];
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
		std::cerr << "Failed to compile shader: " <<
			infoLog << std::endl;
		return false;
	}

	return true;
}

static bool LinkProgram(unsigned int programId) {
	glLinkProgram(programId);

	int success;
	char infoLog[512];
	glGetProgramiv(programId, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(programId, 512, NULL, infoLog);
		std::cerr << "Failed to link program: " <<
			infoLog << std::endl;
		return false;
	}

	return true;
}

Shader::Shader(const std::string& vertexShaderFilePath, const std::string& fragShaderFilePath) {
	std::string vertexShaderStr = ReadFile(vertexShaderFilePath).str();
	std::string fragShaderStr = ReadFile(fragShaderFilePath).str();
	const char* vertexSrc = vertexShaderStr.c_str();
	const char* fragSrc = fragShaderStr.c_str();

	unsigned int vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	unsigned int fragShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vertexShaderId, 1, &vertexSrc, nullptr);
	glShaderSource(fragShaderId, 1, &fragSrc, nullptr);

	bool vertexCompileResult = CompileShader(vertexShaderId);
	if (!vertexCompileResult) {
		throw std::runtime_error("Failed to compile vertex shader");
	}
	bool fragCompileResult = CompileShader(fragShaderId);
	if (!fragCompileResult) {
		throw std::runtime_error("Failed to compile fragment shader");
	}

	m_programId = glCreateProgram();
	glAttachShader(m_programId, vertexShaderId);
	glAttachShader(m_programId, fragShaderId);

	
	bool linkResult = LinkProgram(m_programId);
	if (!linkResult) {
		throw std::runtime_error("Failed to link shader program");
	}

	glDeleteShader(vertexShaderId);
	glDeleteShader(fragShaderId);
}

unsigned int Shader::GetUniformLocation(const std::string& name) {
	if (m_uniformLocationCache.find(name) != m_uniformLocationCache.end()) {
		return m_uniformLocationCache[name];
	}

	unsigned int uniformLocation = glGetUniformLocation(m_programId, name.c_str());
	m_uniformLocationCache[name] = uniformLocation;
	return uniformLocation;
}

void Shader::SetUniformInt(std::string& name, unsigned int value) {
	unsigned int uniformLocation = GetUniformLocation(name);
	glUniform1ui(uniformLocation, value);
}

void Shader::Use() {
	glUseProgram(m_programId);
}