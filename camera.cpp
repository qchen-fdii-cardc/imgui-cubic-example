#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void Camera::init()
{
    // Initialize camera parameters
    cameraDistance = 5.0f;
    rotationX = 0.0f;
    rotationY = 0.0f;
}

void Camera::handleInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    // Handle rotation with arrow keys
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    {
        rotationY -= rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    {
        rotationY += rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        rotationX -= rotationSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        rotationX += rotationSpeed;
    }
}

void Camera::handleScroll(double yoffset)
{
    cameraDistance -= static_cast<float>(yoffset) * scrollSpeed;
    if (cameraDistance < minDistance)
        cameraDistance = minDistance;
    if (cameraDistance > maxDistance)
        cameraDistance = maxDistance;
}

void Camera::setCameraUniforms(GLuint shaderProgram)
{
    // 设置固定的相机位置
    glm::mat4 view = glm::lookAt(
        glm::vec3(0.0f, 0.0f, cameraDistance),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    // 设置视图和投影矩阵
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    // 设置模型矩阵（立方体的旋转）
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(rotationX), glm::vec3(0.0f, 0.0f, 1.0f)); // 绕Z轴旋转
    model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f)); // 绕Y轴旋转
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));
}