#include <glad/glad.h>
#include <GLFW/glfw3.h>
#pragma once
#include <vector>
#include <string>
#include "GraphicsObject.h"
#include "Camera.h"


class Application
{
public:
    void Initialize(int width, int height, const std::string& name);
    void Run();
    void Shutdown();

private:
    void ProcessInput(float deltaTime = 0.0f);

    GLFWwindow* window;

    std::vector<GraphicsObject*> objects; 
    Camera camera;

    int sunIndex;
    int moonIndex;
 //speed controller
    float sunSpinMod = 1.0f;
    float moonOrbitMod = 1.0f;

    //mouse contorl 
    float lastX = 400.0f, lastY = 300.0f;
    bool  firstMouse = true;
 
    int windowWidth  = 800;
    int windowHeight = 600;
};

