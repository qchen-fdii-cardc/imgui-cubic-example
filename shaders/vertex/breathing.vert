/**
 * @file breathing.vert
 * @brief 呼吸效果顶点着色器
 * @details 实现了一个基于时间的呼吸效果，使物体产生缓慢的缩放动画
 * 
 * 功能特点：
 * 1. 基于正弦函数的缩放动画
 * 2. 保持原始颜色信息
 * 3. 可调节的呼吸幅度和速度
 * 4. 支持模型、视图和投影变换
 * 
 * 使用方法：
 * 1. 将此着色器与任何片段着色器配合使用
 * 2. 通过uniform变量time控制动画速度
 * 3. 适合需要呼吸动画效果的场景
 * 
 * 参数说明：
 * - time: 控制呼吸动画的时间变量
 * - model: 模型变换矩阵
 * - view: 视图变换矩阵
 * - projection: 投影变换矩阵
 * 
 * 自定义修改：
 * 1. 调整呼吸幅度：修改sin函数前的系数(0.2)
 * 2. 调整呼吸速度：修改time的乘数(2.0)
 * 3. 改变缩放方向：修改特定轴的缩放
 * 4. 添加多轴呼吸：为不同轴设置不同的呼吸效果
 */

#version 330 core
// 顶点位置输入，location=0表示这是第一个顶点属性
layout (location = 0) in vec3 aPos;
// 顶点颜色输入，location=1表示这是第二个顶点属性
layout (location = 1) in vec3 aColor;

// 输出到片段着色器的颜色
out vec3 vertexColor;

// 变换矩阵
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
// 时间uniform变量，用于控制动画
uniform float time;

void main()
{
    // 首先传递颜色，在进行任何变换之前
    vertexColor = aColor;
    
    // 创建呼吸效果
    // sin(time * 2.0)生成-1到1的波动
    // * 0.2将幅度限制在20%
    // + 1.0确保缩放范围在0.8到1.2之间
    float breathingScale = 1.0 + 0.2 * sin(time * 2.0);
    
    // 仅对位置应用呼吸效果
    vec3 scaledPos = aPos * breathingScale;
    
    // 计算最终位置
    gl_Position = projection * view * model * vec4(scaledPos, 1.0);
} 