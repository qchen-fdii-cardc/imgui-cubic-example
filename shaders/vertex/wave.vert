/**
 * @file wave.vert
 * @brief 波浪效果顶点着色器
 * @details 实现了一个基于正弦函数的波浪动画效果，使立方体表面产生波浪状的变形
 * 
 * 功能特点：
 * 1. 基于时间的动态波浪效果
 * 2. 可调节的波浪幅度和频率
 * 3. 保持原始颜色信息
 * 4. 支持模型、视图和投影变换
 * 
 * 使用方法：
 * 1. 将此着色器与任何片段着色器配合使用
 * 2. 通过uniform变量time控制动画速度
 * 3. 可以通过修改sin函数的参数来调整波浪效果
 * 
 * 参数说明：
 * - time: 控制波浪动画的时间变量
 * - model: 模型变换矩阵
 * - view: 视图变换矩阵
 * - projection: 投影变换矩阵
 * 
 * 自定义修改：
 * 1. 调整波浪幅度：修改sin函数前的系数(0.2)
 * 2. 调整波浪频率：修改pos.x前的系数(2.0)
 * 3. 改变波浪方向：修改pos.x为pos.z或其他坐标
 * 4. 添加多方向波浪：叠加多个sin函数
 */

#version 330 core
// 顶点位置输入，location=0表示这是第一个顶点属性
layout (location = 0) in vec3 aPos;
// 顶点颜色输入，location=1表示这是第二个顶点属性
layout (location = 1) in vec3 aColor;

// 输出到片段着色器的颜色
out vec3 vertexColor;

// 时间uniform变量，用于控制动画
uniform float time;
// 模型变换矩阵
uniform mat4 model;
// 视图变换矩阵
uniform mat4 view;
// 投影变换矩阵
uniform mat4 projection;

void main()
{
    // 保存原始位置
    vec3 pos = aPos;
    // 添加基于时间的正弦波动效果
    // sin(time + pos.x * 2.0) * 0.2 计算波浪偏移
    // time: 控制波浪移动速度
    // pos.x * 2.0: 控制波浪频率
    // 0.2: 控制波浪幅度
    pos.y += sin(time + pos.x * 2.0) * 0.2;
    
    // 应用变换矩阵并输出最终位置
    gl_Position = projection * view * model * vec4(pos, 1.0);
    
    // 传递原始颜色到片段着色器
    vertexColor = aColor;
} 