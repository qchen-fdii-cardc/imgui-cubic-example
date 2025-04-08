/**
 * @file ui.h
 * @brief 用户界面系统头文件
 * @details 定义了用户界面管理类，负责ImGui界面的创建、渲染和交互
 */

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

/**
 * @class UI
 * @brief 用户界面管理类，使用单例模式实现
 * @details 负责创建和管理ImGui界面，处理用户交互和着色器参数调节
 */
class UI
{
public:
    /**
     * @brief 获取UI单例实例
     * @return UI& 单例实例的引用
     */
    static UI &getInstance()
    {
        static UI instance;
        return instance;
    }

    /**
     * @brief 初始化UI系统
     * @param window GLFW窗口指针
     * @details 初始化ImGui上下文和设置
     */
    void init(GLFWwindow *window);

    /**
     * @brief 渲染UI界面
     * @param currentVertexShader 当前顶点着色器索引指针
     * @param currentFragmentShader 当前片段着色器索引指针
     * @details 渲染ImGui界面并处理着色器选择
     */
    void render(int *currentVertexShader, int *currentFragmentShader);

    /**
     * @brief 清理UI资源
     * @details 清理ImGui上下文和资源
     */
    void cleanup();

    /**
     * @brief 处理输入事件
     * @param window GLFW窗口指针
     * @details 处理UI相关的输入事件
     */
    void handleInput(GLFWwindow *window);

private:
    // 私有构造函数和析构函数，确保单例模式
    UI() : currentVertexShader(0), currentFragmentShader(0) {}
    ~UI() = default;

    // 删除拷贝构造函数和赋值运算符
    UI(const UI &) = delete;
    UI &operator=(const UI &) = delete;

    /**
     * @brief 将角度标准化到0-360范围
     * @param angle 输入角度
     * @return float 标准化后的角度
     */
    float normalizeAngle(float angle) const;

    int currentVertexShader;   // 当前顶点着色器索引
    int currentFragmentShader; // 当前片段着色器索引
};