/**
 * @file camera.h
 * @brief 相机系统头文件
 * @details 定义了相机管理类，负责处理视角控制和变换矩阵计算
 */

#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <string>

/**
 * @class Camera
 * @brief 相机管理类，使用单例模式实现
 * @details 负责处理相机视角控制、输入响应和变换矩阵计算
 */
class Camera
{
public:
    /**
     * @brief 获取Camera单例实例
     * @return Camera& 单例实例的引用
     */
    static Camera &getInstance()
    {
        static Camera instance;
        return instance;
    }

    /**
     * @brief 初始化相机系统
     * @details 设置相机初始参数
     */
    void init();

    /**
     * @brief 处理键盘输入
     * @param window GLFW窗口指针
     * @details 响应方向键输入，控制立方体旋转
     */
    void handleInput(GLFWwindow *window);

    /**
     * @brief 处理鼠标滚轮输入
     * @param yoffset 滚轮偏移量
     * @details 控制相机距离，实现缩放效果
     */
    void handleScroll(double yoffset);

    /**
     * @brief 设置相机相关的uniform变量
     * @param shaderProgram 当前使用的着色器程序ID
     * @details 将视图和投影矩阵传递给着色器
     */
    void setCameraUniforms(GLuint shaderProgram);

    // Getters for UI display
    /**
     * @brief 获取X轴旋转角度
     * @return float X轴旋转角度
     */
    float getRotationX() const { return rotationX; }

    /**
     * @brief 获取Y轴旋转角度
     * @return float Y轴旋转角度
     */
    float getRotationY() const { return rotationY; }

    /**
     * @brief 获取相机距离
     * @return float 相机到物体的距离
     */
    float getCameraDistance() const { return cameraDistance; }

private:
    // 私有构造函数和析构函数，确保单例模式
    Camera() = default;
    ~Camera() = default;

    // 删除拷贝构造函数和赋值运算符
    Camera(const Camera &) = delete;
    Camera &operator=(const Camera &) = delete;

    float cameraDistance = 5.0f;      // 相机到物体的距离
    float rotationX = 0.0f;           // 立方体绕X轴旋转角度
    float rotationY = 0.0f;           // 立方体绕Y轴旋转角度
    const float rotationSpeed = 2.0f; // 旋转速度
    const float minDistance = 2.0f;   // 最小相机距离
    const float maxDistance = 10.0f;  // 最大相机距离
    const float scrollSpeed = 0.5f;   // 滚轮缩放速度
};