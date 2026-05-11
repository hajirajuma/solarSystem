#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cstddef>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "../headers/Application.h"
#include "../headers/Texture.h"
#include "../headers/SphereObject.h"

unsigned int shaderProgram = 0;

Texture sunTex, mercuryTex, venusTex, earthTex, moonTex, marsTex;

unsigned int LoadShader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;

    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);

    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();

    const char* vCode = vertexCode.c_str();
    const char* fCode = fragmentCode.c_str();

    unsigned int vertex, fragment;

    // vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vCode, NULL);
    glCompileShader(vertex);

    // fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fCode, NULL);
    glCompileShader(fragment);

    
    int success;
    char infoLog[1024];
    glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertex, 1024, NULL, infoLog);
        std::cout << "Vertex shader compilation failed:\n" << infoLog << std::endl;
    }
    glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment, 1024, NULL, infoLog);
        std::cout << "Fragment shader compilation failed:\n" << infoLog << std::endl;
    }

    unsigned int ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 1024, NULL, infoLog);
        std::cout << "Shader program linking failed:\n" << infoLog << std::endl;
    }

    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return ID;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void Application::Initialize(int width, int height, const std::string& name)
{
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback); 

    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glEnable(GL_DEPTH_TEST);

    // loadiing textures
    sunTex.Load("PlanetTextureMaps/sunmap.jpg");
    mercuryTex.Load("PlanetTextureMaps/mercurymap.jpg");
    venusTex.Load("PlanetTextureMaps/venusmap.jpg");
    earthTex.Load("PlanetTextureMaps/earthmap1k.jpg");
    marsTex.Load("PlanetTextureMaps/marsmap1k.jpg");
    moonTex.Load("PlanetTextureMaps/moon.jpg");

    shaderProgram = LoadShader("shaders/vertex.glsl", "shaders/fragment.glsl");

    // create objects
    SphereObject* sunObj = new SphereObject();
    SphereObject* mercuryObj = new SphereObject();
    SphereObject* venusObj = new SphereObject();
    SphereObject* earthObj = new SphereObject();
    SphereObject* marsObj = new SphereObject();
    SphereObject* moonObj = new SphereObject();

    // setting properties
    sunObj->orbitSpeed = 0.0f; sunObj->rotationSpeed = 0.5f; sunObj->distance = 0; sunObj->scale = 2.0f; sunObj->parent = nullptr;
    mercuryObj->orbitSpeed = 1.2f; mercuryObj->rotationSpeed = 2.0f; mercuryObj->distance = 3.0f; mercuryObj->scale = 0.3f; mercuryObj->parent = sunObj;
    venusObj->orbitSpeed = 0.9f; venusObj->rotationSpeed = 1.5f; venusObj->distance = 4.5f; venusObj->scale = 0.5f; venusObj->parent = sunObj;
    earthObj->orbitSpeed = 0.7f; earthObj->rotationSpeed = 2.5f; earthObj->distance = 6.0f; earthObj->scale = 0.6f; earthObj->parent = sunObj;
    marsObj->orbitSpeed = 0.5f; marsObj->rotationSpeed = 1.2f; marsObj->distance = 7.5f; marsObj->scale = 0.5f; marsObj->parent = sunObj;
    moonObj->orbitSpeed = 2.0f; moonObj->rotationSpeed = 3.0f; moonObj->distance = 1.0f; moonObj->scale = 0.2f; moonObj->parent = earthObj;

    // initialise objects
    sunObj->Initialize(shaderProgram, sunTex.ID);
    mercuryObj->Initialize(shaderProgram, mercuryTex.ID);
    venusObj->Initialize(shaderProgram, venusTex.ID);
    earthObj->Initialize(shaderProgram, earthTex.ID);
    marsObj->Initialize(shaderProgram, marsTex.ID);
    moonObj->Initialize(shaderProgram, moonTex.ID);
    // adding objects to list
    objects.push_back(sunObj);
    objects.push_back(mercuryObj);
    objects.push_back(venusObj);
    objects.push_back(earthObj);
    objects.push_back(marsObj);
    objects.push_back(moonObj);
}

void Application::Run()
{
    while (!glfwWindowShouldClose(window))
    {
        ProcessInput();

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(shaderProgram);

        float time = glfwGetTime();
        //glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 3.0f, -15.0f));
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        //  lighting
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
        glUniform3f(glGetUniformLocation(shaderProgram, "lightDir"), 0.0f, 0.0f, -1.0f);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLightPos"), 0.0f, 0.0f, 0.0f);

        //camera position for lighting
        glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(camera.Position));
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

        // yellow sunlight
        glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.direction"),-0.5f, -1.0f, -0.5f);
        glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.ambient"), 0.15f, 0.15f, 0.05f);
        glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.diffuse"),0.9f,  0.85f, 0.2f);   
        glUniform3f(glGetUniformLocation(shaderProgram, "dirLight.specular"),0.5f,  0.5f,  0.1f);
        float plTime = (float)glfwGetTime();
        glm::vec3 pointLightPos(
            8.0f * cos(plTime * 0.4f),
            4.0f,
            8.0f * sin(plTime * 0.4f)
        );
        glUniform3fv(glGetUniformLocation(shaderProgram, "pointLight.position"), 1, glm::value_ptr(pointLightPos));
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.ambient"), 0.0f, 0.0f, 0.1f);
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.diffuse"), 0.2f, 0.2f, 1.0f);   
        glUniform3f(glGetUniformLocation(shaderProgram, "pointLight.specular"), 0.5f, 0.5f, 1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLight.constant"),  1.0f);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLight.linear"),    0.07f);
        glUniform1f(glGetUniformLocation(shaderProgram, "pointLight.quadratic"), 0.017f);

        for (size_t i = 0; i < objects.size(); ++i)
        {
            objects[i]->Update(time);
            objects[i]->Draw();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::ProcessInput(float deltaTime)
{
     if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
 
    // Camera movement
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(0, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(1, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(2, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(3, deltaTime);
 
    // Up and Down arrows of sun
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
        sunSpinMod += 0.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
        sunSpinMod = glm::max(0.0f, sunSpinMod - 0.5f * deltaTime);
 
    // Left and Right arrows of moon
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
        moonOrbitMod += 0.5f * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
        moonOrbitMod = glm::max(0.0f, moonOrbitMod - 0.5f * deltaTime);
}

void Application::Shutdown()
{
    for (size_t i = 0; i < objects.size(); ++i)
    {
        objects[i]->Cleanup();
        delete objects[i];
    }

    glfwTerminate();
}