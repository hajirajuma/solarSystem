#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#pragma once
#include "GraphicsObject.h"

class CubeObject : public GraphicsObject
{
public:
    void Initialize(unsigned int shader, unsigned int texture) override;
    void Update(float time) override;
    void Draw() override;
    void Cleanup() override;
};