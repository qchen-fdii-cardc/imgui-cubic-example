/**
 * @file normal.vert
 * @brief 基础顶点着色器
 * @details 实现了一个基础的顶点着色器，提供标准的顶点变换和颜色传递功能
 * 
 * 功能特点：
 * 1. 标准的MVP（Model-View-Projection）变换
 * 2. 简单的颜色传递
 * 3. 无特殊效果，保持原始几何形状
 * 
 * 使用方法：
 * 1. 作为基础顶点着色器使用
 * 2. 与其他片段着色器配合
 * 3. 适合需要保持原始形状的场景
 * 
 * 参数说明：
 * - model: 模型变换矩阵，控制物体的位置、旋转和缩放
 * - view: 视图变换矩阵，控制相机视角
 * - projection: 投影变换矩阵，控制投影方式
 * 
 * 自定义修改：
 * 1. 添加顶点动画：参考wave.vert的实现
 * 2. 修改变换顺序：调整MVP矩阵的乘法顺序
 * 3. 添加其他顶点属性：如法线、纹理坐标等
 * 4. 实现顶点颜色计算：在传递前对颜色进行处理
 */

#version 330 core
// 顶点位置输入，location=0表示这是第一个顶点属性
layout (location = 0) in vec3 aPos;
// 顶点颜色输入，location=1表示这是第二个顶点属性
layout (location = 1) in vec3 aColor;

// 输出到片段着色器的颜色
out vec3 vertexColor;

// 模型变换矩阵
uniform mat4 model;
// 视图变换矩阵
uniform mat4 view;
// 投影变换矩阵
uniform mat4 projection;

void main()
{
    // 应用MVP变换并输出最终位置
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    // 直接传递原始颜色到片段着色器
    vertexColor = aColor;
} 