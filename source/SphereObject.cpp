#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "../headers/SphereObject.h"
#include "../headers/Sphere.h"
#include <iostream>



void SphereObject::Initialize(unsigned int shader, unsigned int texture)
{
    this->shaderProgram = shader;
    this->textureID = texture;

    Sphere sphere(1.0f, 36, 18);
    indexCount = sphere.getIndexCount();

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sphere.getInterleavedVertexSize(),sphere.getInterleavedVertices(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere.getIndexSize(),sphere.getIndices(), GL_STATIC_DRAW);

    int stride = sphere.getInterleavedStride();
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3*sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6*sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
}

void SphereObject::Update(float time)
{
    transform = glm::mat4(1.0f);

    if (parent != nullptr)
        transform = parent->transform;

    transform = glm::rotate(transform, time * orbitSpeed, glm::vec3(0,1,0));
    transform = glm::translate(transform, glm::vec3(distance, 0, 0));
    transform = glm::scale(transform, glm::vec3(scale));
    transform = glm::rotate(transform, time * rotationSpeed, glm::vec3(0,1,0));
}

void SphereObject::Draw()
{
    glUseProgram(shaderProgram);

    glBindTexture(GL_TEXTURE_2D, textureID);

    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),
        1, GL_FALSE, glm::value_ptr(transform));

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, 0);
}

void SphereObject::Cleanup()
{
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
