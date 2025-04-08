/**
 * @file rainbow.frag
 * @brief 彩虹效果片段着色器
 * @details 实现了一个基于时间的彩虹效果，使物体表面产生流动的彩虹色彩
 * 
 * 功能特点：
 * 1. 基于时间的动态彩虹效果
 * 2. 保持原始颜色的部分特征
 * 3. 基于面的不同产生不同的彩虹效果
 * 4. 平滑的颜色过渡
 * 
 * 使用方法：
 * 1. 与任何顶点着色器配合使用
 * 2. 通过uniform变量time控制动画速度
 * 3. 适合需要炫彩效果的场景
 * 
 * 参数说明：
 * - vertexColor: 从顶点着色器传入的基础颜色
 * - time: 控制彩虹动画的时间变量
 * 
 * 自定义修改：
 * 1. 调整彩虹速度：修改time的乘数(0.5)
 * 2. 调整颜色混合比例：修改mix函数的0.7系数
 * 3. 改变颜色相位：修改phase的计算方式
 * 4. 添加更多颜色变化：修改sin函数的相位差
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
    // 使用原始颜色作为基础
    vec3 baseColor = vertexColor;
    
    // 根据面的不同（通过原始颜色的差异）应用不同的彩虹效果
    // 使用颜色的各个分量创建不同的相位
    float faceIndex = baseColor.x + baseColor.y * 2.0 + baseColor.z * 3.0;
    // 使用面的索引创建不同的相位，6.28 = 2 * PI
    float phase = mod(faceIndex * 2.0, 6.28);
    
    // 创建彩虹效果
    // 使用不同相位的正弦函数生成RGB颜色
    // 2.094和4.189分别是2π/3和4π/3，用于创建120度的相位差
    float r = sin(time * 0.5 + phase) * 0.5 + 0.5;
    float g = sin(time * 0.5 + phase + 2.094) * 0.5 + 0.5;
    float b = sin(time * 0.5 + phase + 4.189) * 0.5 + 0.5;
    
    // 混合原始颜色和彩虹效果
    vec3 rainbowColor = vec3(r, g, b);
    // 70%彩虹效果，30%原始颜色
    vec3 finalColor = mix(baseColor, rainbowColor, 0.7);
    
    // 输出最终颜色，不透明度保持为1.0
    FragColor = vec4(finalColor, 1.0);
} 