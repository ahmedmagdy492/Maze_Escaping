#pragma once

#include <iostream>
#include <unordered_map>

class Shader {
private:
	unsigned int m_programId;
	std::unordered_map<std::string, unsigned int> m_uniformLocationCache;

	unsigned int GetUniformLocation(const std::string& name);

public:
	Shader(const std::string& vertexShaderFilePath, const std::string& fragShaderFilePath);

	void SetUniformInt(std::string& name, unsigned int value);
	void SetUniformMat4(std::string& name, float* value);

	void Use();
};