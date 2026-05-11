#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <glm/glm.hpp>
#pragma once
#include "GraphicsObject.h"


class SphereObject : public GraphicsObject
{
public:
    float orbitSpeed;
    float rotationSpeed;
    float distance;
    float scale;

    float* orbitSpeedMod    = nullptr;  
    float* rotationSpeedMod = nullptr;   

    SphereObject* parent;

    void Initialize(unsigned int shader, unsigned int texture) override;
    void Update(float time) override;
    void Draw() override;
    void Cleanup() override;
};


