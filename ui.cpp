#include "ui.h"
#include "camera.h"
#include "shader.h"
#include "imgui.h"
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <GLFW/glfw3.h>
#include <iostream>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

float UI::normalizeAngle(float angle) const
{
    // Convert to degrees
    float degrees = angle * 180.0f / M_PI;

    // Normalize to 0-360 range
    while (degrees < 0.0f)
        degrees += 360.0f;
    while (degrees >= 360.0f)
        degrees -= 360.0f;

    return degrees;
}

void UI::init(GLFWwindow *window)
{
    // 初始化 ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    // 设置 ImGui 后端
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void UI::render(int *currentVertexShaderPtr, int *currentFragmentShaderPtr)
{
    // 开始 ImGui 帧
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 创建控制面板
    ImGui::Begin("Shader Control");

    // 获取当前 shader 程序名称
    std::string currentShaderName = "normal_normal"; // 默认值
    for (const auto &program : Shader::getInstance().getShaderPrograms())
    {
        if (program.second == Shader::getInstance().getCurrentProgram())
        {
            currentShaderName = program.first;
            break;
        }
    }

    // 显示当前 shader 程序
    ImGui::Text("Current Shader: %s", currentShaderName.c_str());

    // 添加分隔线
    ImGui::Separator();

    // 显示旋转角度
    float xAngle = Camera::getInstance().getRotationX();
    float yAngle = Camera::getInstance().getRotationY();
    ImGui::Text("Rotation X: %.1f°", normalizeAngle(xAngle));
    ImGui::Text("Rotation Y: %.1f°", normalizeAngle(yAngle));

    // 显示相机距离
    ImGui::Text("Camera Distance: %.1f", Camera::getInstance().getCameraDistance());

    // 添加分隔线
    ImGui::Separator();

    // 添加重置按钮
    if (ImGui::Button("Reset to Default"))
    {
        // 重置相机状态
        Camera::getInstance().init();

        // 重置着色器选择
        *currentVertexShaderPtr = 0;   // normal
        *currentFragmentShaderPtr = 0; // normal
        Shader::getInstance().setCurrentProgram("normal_normal");
    }

    // 添加分隔线
    ImGui::Separator();

    // 添加 shader 选择下拉菜单
    const char *vertexShaders[] = {"normal", "wave", "breathing"};
    const char *fragmentShaders[] = {"normal", "pulse", "rainbow"};

    // 使用传入的指针更新当前选择的着色器
    if (ImGui::Combo("Vertex Shader", currentVertexShaderPtr, vertexShaders, IM_ARRAYSIZE(vertexShaders)))
    {
        // 当顶点 shader 改变时，更新 shader 程序
        std::string newShaderName = std::string(vertexShaders[*currentVertexShaderPtr]) + "_" +
                                    std::string(fragmentShaders[*currentFragmentShaderPtr]);
        Shader::getInstance().setCurrentProgram(newShaderName);
    }

    if (ImGui::Combo("Fragment Shader", currentFragmentShaderPtr, fragmentShaders, IM_ARRAYSIZE(fragmentShaders)))
    {
        // 当片段 shader 改变时，更新 shader 程序
        std::string newShaderName = std::string(vertexShaders[*currentVertexShaderPtr]) + "_" +
                                    std::string(fragmentShaders[*currentFragmentShaderPtr]);
        Shader::getInstance().setCurrentProgram(newShaderName);
    }

    ImGui::End();

    // 渲染 ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void UI::cleanup()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void UI::handleInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);
}