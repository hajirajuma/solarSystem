#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <string>
#pragma once


class GraphicsObject
{
public:
    virtual void Initialize(unsigned int shader, unsigned int texture) = 0;
    virtual void Update(float time) = 0;
    virtual void Draw() = 0;
    virtual void Cleanup() = 0;

protected:
    glm::mat4 transform;

    unsigned int VAO, VBO, EBO;
    unsigned int shaderProgram;  
    unsigned int textureID;
    unsigned int indexCount;

    GraphicsObject()
{
    shaderProgram = 0;
    textureID = 0;
}
};