/**
 * @file shader.h
 * @brief 着色器系统头文件
 * @details 定义了着色器管理类，负责着色器程序的加载、编译和管理
 */

#pragma once

#include <GL/glew.h>
#include <string>
#include <vector>
#include <unordered_map>
#include <glm/glm.hpp>
#include "SimpleIni.h"

/**
 * @class Shader
 * @brief 着色器管理类，使用单例模式实现
 * @details 负责着色器程序的加载、编译、管理和uniform变量设置
 */
class Shader
{
public:
    /**
     * @brief 获取Shader单例实例
     * @return Shader& 单例实例的引用
     */
    static Shader &getInstance()
    {
        static Shader instance;
        return instance;
    }

    /**
     * @brief 初始化着色器系统
     * @details 加载着色器配置文件并初始化着色器程序
     */
    void init();

    /**
     * @brief 清理着色器资源
     * @details 删除所有已创建的着色器程序
     */
    void cleanup();

    /**
     * @brief 使用当前着色器程序
     * @details 激活当前选中的着色器程序
     */
    void use();

    /**
     * @brief 设置float类型的uniform变量
     * @param name uniform变量名
     * @param value 要设置的值
     */
    void setUniform(const std::string &name, float value);

    /**
     * @brief 设置vec3类型的uniform变量
     * @param name uniform变量名
     * @param value 要设置的值
     */
    void setUniform(const std::string &name, const glm::vec3 &value);

    /**
     * @brief 设置mat4类型的uniform变量
     * @param name uniform变量名
     * @param value 要设置的值
     */
    void setUniform(const std::string &name, const glm::mat4 &value);

    /**
     * @brief 设置int类型的uniform变量
     * @param name uniform变量名
     * @param value 要设置的值
     */
    void setUniform(const std::string &name, int value);

    /**
     * @brief 设置bool类型的uniform变量
     * @param name uniform变量名
     * @param value 要设置的值
     */
    void setUniform(const std::string &name, bool value);

    /**
     * @brief 获取当前使用的着色器程序ID
     * @return GLuint 当前着色器程序ID
     */
    GLuint getCurrentProgram() const { return currentProgram; }

    /**
     * @brief 获取所有着色器程序的映射
     * @return const std::unordered_map<std::string, GLuint>& 着色器程序映射
     */
    const std::unordered_map<std::string, GLuint> &getShaderPrograms() const { return shaderPrograms; }

    /**
     * @brief 设置当前使用的着色器程序
     * @param name 着色器程序名称
     */
    void setCurrentProgram(const std::string &name);

    /**
     * @brief 使用指定的顶点和片段着色器程序
     * @param vertexShaderIndex 顶点着色器索引
     * @param fragmentShaderIndex 片段着色器索引
     */
    void useShaderProgram(int vertexShaderIndex, int fragmentShaderIndex);

    /**
     * @brief 创建新的着色器程序
     * @param vertexPath 顶点着色器文件路径
     * @param fragmentPath 片段着色器文件路径
     * @return GLuint 创建的着色器程序ID
     */
    GLuint createShaderProgram(const std::string &vertexPath, const std::string &fragmentPath);

    /**
     * @brief 删除着色器程序
     * @param program 要删除的着色器程序ID
     */
    void deleteShaderProgram(GLuint program);

private:
    // 私有构造函数和析构函数，确保单例模式
    Shader() : currentProgram(0) {}
    ~Shader() { cleanup(); }
    Shader(const Shader &) = delete;
    Shader &operator=(const Shader &) = delete;

    /**
     * @brief 创建着色器程序
     * @param vertexPath 顶点着色器文件路径
     * @param fragmentPath 片段着色器文件路径
     * @return GLuint 创建的着色器程序ID
     */
    GLuint createShader(const std::string &vertexPath, const std::string &fragmentPath);

    /**
     * @brief 编译着色器
     * @param source 着色器源代码
     * @param type 着色器类型
     * @return GLuint 编译后的着色器ID
     */
    GLuint compileShader(const std::string &source, GLenum type);

    /**
     * @brief 加载着色器源代码
     * @param path 着色器文件路径
     * @return std::string 着色器源代码
     */
    std::string loadShaderSource(const std::string &path);

    /**
     * @brief 从INI文件加载着色器路径
     * @param filename INI文件名
     * @return bool 是否加载成功
     */
    bool loadShaderPathsFromIni(const std::string &filename);

    /**
     * @brief 检查着色器编译错误
     * @param shader 着色器ID
     * @param type 着色器类型
     */
    void checkShaderErrors(GLuint shader, const std::string &type);

    GLuint currentProgram = 0;                              // 当前使用的着色器程序ID
    std::unordered_map<std::string, GLuint> shaderPrograms; // 着色器程序映射

    std::unordered_map<std::string, std::string> vertexShaderPaths;   // 顶点着色器路径映射
    std::unordered_map<std::string, std::string> fragmentShaderPaths; // 片段着色器路径映射
};