#pragma once
#include <string>

class Shader
{
public:
    unsigned int shaderProgram;

    Shader(const char* vertexPath, const char* fragmentPath);

    void Use();

    void SetMat4(const std::string& name, const float* value);
};