/**
 * @file main.cpp
 * @brief 程序入口和主循环实现
 * @details 实现了应用程序的核心功能，包括初始化、主循环和资源清理
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include <imgui/backends/imgui_impl_glfw.h>
#include <imgui/backends/imgui_impl_opengl3.h>
#include <iostream>
#include <memory>
#include "shader.h"
#include "camera.h"
#include "cube.h"
#include "ui.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/**
 * @class Application
 * @brief 应用程序主类，使用单例模式实现
 * @details 负责协调各个模块的工作，管理程序的生命周期
 */
class Application
{
public:
    /**
     * @brief 获取Application单例实例
     * @return Application& 单例实例的引用
     */
    static Application &getInstance()
    {
        static Application instance;
        return instance;
    }

    /**
     * @brief 初始化应用程序
     * @return bool 初始化是否成功
     * @details 初始化GLFW、OpenGL上下文、各个模块等
     */
    bool init()
    {
        // 初始化GLFW
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return false;
        }

        // 配置GLFW
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // 创建窗口
        window = glfwCreateWindow(800, 600, "Shader Demo", nullptr, nullptr);
        if (!window)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return false;
        }

        glfwMakeContextCurrent(window);

        // 初始化GLEW
        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            return false;
        }

        // 设置回调函数
        glfwSetFramebufferSizeCallback(window, [](GLFWwindow *w, int width, int height)
                                       { glViewport(0, 0, width, height); });

        glfwSetScrollCallback(window, [](GLFWwindow *w, double xoffset, double yoffset)
                              { Camera::getInstance().handleScroll(yoffset); });

        // 初始化各个模块
        Shader::getInstance().init();
        UI::getInstance().init(window);
        Camera::getInstance().init();
        Cube::getInstance().init();

        // 启用深度测试
        glEnable(GL_DEPTH_TEST);

        return true;
    }

    /**
     * @brief 运行应用程序主循环
     * @details 处理渲染循环、输入事件和UI更新
     */
    void run()
    {
        while (!glfwWindowShouldClose(window))
        {
            // 处理相机输入
            Camera::getInstance().handleInput(window);

            // 清除缓冲区
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // 使用当前着色器程序
            Shader::getInstance().useShaderProgram(currentVertexShader, currentFragmentShader);

            // 设置时间uniform变量
            float timeValue = glfwGetTime();
            GLuint currentProgram = Shader::getInstance().getCurrentProgram();
            int timeLocation = glGetUniformLocation(currentProgram, "time");
            if (timeLocation != -1)
            {
                glUniform1f(timeLocation, timeValue);
            }

            // 设置相机uniform变量
            Camera::getInstance().setCameraUniforms(currentProgram);

            // 设置模型矩阵
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(Camera::getInstance().getRotationX()), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(Camera::getInstance().getRotationY()), glm::vec3(0.0f, 1.0f, 0.0f));
            glUniformMatrix4fv(glGetUniformLocation(currentProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

            // 渲染立方体
            Cube::getInstance().render();

            // 渲染UI
            UI::getInstance().render(&currentVertexShader, &currentFragmentShader);

            // 交换缓冲区和处理事件
            glfwSwapBuffers(window);
            glfwPollEvents();
        }
    }

    /**
     * @brief 清理应用程序资源
     * @details 清理各个模块的资源并终止GLFW
     */
    void cleanup()
    {
        UI::getInstance().cleanup();
        Shader::getInstance().cleanup();
        Cube::getInstance().cleanup();
        glfwTerminate();
    }

private:
    // 私有构造函数和析构函数，确保单例模式
    Application() = default;
    ~Application() = default;

    // 删除拷贝构造函数和赋值运算符
    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    GLFWwindow *window = nullptr;
    int currentVertexShader = 0;
    int currentFragmentShader = 0;
};

/**
 * @brief 程序入口点
 * @return int 程序退出码
 */
int main()
{
    auto &app = Application::getInstance();

    if (!app.init())
    {
        return -1;
    }

    app.run();
    app.cleanup();

    return 0;
}
