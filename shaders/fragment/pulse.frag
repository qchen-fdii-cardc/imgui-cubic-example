/**
 * @file pulse.frag
 * @brief 脉冲效果片段着色器
 * @details 实现了一个基于时间的脉冲效果，使物体颜色随时间产生明暗变化
 * 
 * 功能特点：
 * 1. 基于正弦函数的颜色脉冲效果
 * 2. 保持原始颜色色调
 * 3. 可调节的脉冲速度和幅度
 * 4. 平滑的颜色过渡
 * 
 * 使用方法：
 * 1. 与任何顶点着色器配合使用
 * 2. 通过uniform变量time控制动画速度
 * 3. 适合需要呼吸灯效果的场景
 * 
 * 参数说明：
 * - vertexColor: 从顶点着色器传入的基础颜色
 * - time: 控制脉冲动画的时间变量
 * 
 * 自定义修改：
 * 1. 调整脉冲速度：修改time的乘数(2.0)
 * 2. 调整脉冲幅度：修改pulse的系数(0.5)
 * 3. 改变颜色混合方式：修改mix函数的混合比例
 * 4. 添加多色脉冲：为RGB通道设置不同的相位
 */

#version 330 core
// 从顶点着色器接收的颜色
in vec3 vertexColor;
// 输出的片段颜色
out vec4 FragColor;

// 时间uniform变量，用于控制动画
uniform float time;

void main()
{
    // 创建脉冲效果
    // sin(time * 2.0)生成-1到1的波动
    // * 0.5 + 0.5将范围映射到0到1
    float pulse = sin(time * 2.0) * 0.5 + 0.5;
    
    // 将原始颜色与脉冲效果混合
    // 0.5 + pulse * 0.5确保颜色在50%到100%之间变化
    vec3 color = vertexColor * (0.5 + pulse * 0.5);
    
    // 输出最终颜色，不透明度保持为1.0
    FragColor = vec4(color, 1.0);
} 