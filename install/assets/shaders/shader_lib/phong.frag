#version 460

layout(location = 0) out vec4 o_fragColor;

layout(location = 0) in vec3 v_normal;
layout(location = 1) in vec2 v_uv;

uniform vec4 u_color;         // 基础颜色

uniform sampler2D u_textures[16];   // 最多支持16个纹理
uniform float u_blendFactors[16];   // 混合因子
uniform int u_textureCount;         // 纹理数量


uniform vec3 u_lightColor;      // 光源颜色

void main() {
    vec4 finalColor = vec4(0.0);

    vec3 blendedColor = vec3(0.0);  // 初始化混合颜色为黑色

    // 根据纹理数量计算颜色
    for(int i = 0; i < u_textureCount; i++) {
        blendedColor += texture(u_textures[i], v_uv).rgb * u_blendFactors[i];
    }

    // 混合基础颜色（alpha 通道用于控制基础颜色的影响）
    blendedColor = mix(blendedColor, u_color.rgb, u_color.a);


    float ambientStrength = 1.0;
    vec3 ambient = ambientStrength * u_lightColor;

    vec3 result = ambient * blendedColor;

    finalColor = vec4(result, u_color.a);

    // 输出最终颜色，alpha 值为基础颜色的 alpha
    o_fragColor = finalColor;
}
