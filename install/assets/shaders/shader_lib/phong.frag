#version 460

layout(location = 0) out vec4 o_fragColor;

layout(location = 0) in vec3 v_normal;
layout(location = 1) in vec2 v_uv;
layout(location = 2) in vec3 v_worldPosition;

uniform vec4 u_color;         // 基础颜色

uniform vec3 u_cameraPosition;  // 相机位置

uniform sampler2D u_textures[16];   // 最多支持16个纹理
uniform float u_blendFactors[16];   // 混合因子
uniform int u_textureCount;         // 纹理数量

uniform vec3 u_lightColor;      // 光源颜色
uniform vec3 u_lightPosition;   // 光源位置



void main() {
    vec4 finalColor = vec4(0.0);

    vec3 blendedColor = vec3(0.0);  // 初始化混合颜色为黑色

    // 根据纹理数量计算颜色
    for(int i = 0; i < u_textureCount; i++) {
        blendedColor += texture(u_textures[i], v_uv).rgb * u_blendFactors[i];
    }

    // 混合基础颜色（alpha 通道用于控制基础颜色的影响）
    blendedColor = mix(blendedColor, u_color.rgb, u_color.a);

    // 计算光照
    // 环境光
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * u_lightColor;

    // 漫反射光
    vec3 norm = normalize(v_normal);
    vec3 lightDir = normalize(v_worldPosition - u_lightPosition);
    float diff = max(dot(norm, -lightDir), 0.0);
    vec3 diffuse = diff * u_lightColor;

    // 镜面光
    float specularStrength = 0.5;
    vec3 viewDir = normalize(v_worldPosition - u_cameraPosition);
    vec3 halfwayDir = normalize((-lightDir) + (-viewDir));
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 128.0);
    vec3 specular = specularStrength * spec * u_lightColor;

    vec3 result = ambient + diffuse + specular;
    finalColor = vec4(result * blendedColor, u_color.a);

    // 输出最终颜色，alpha 值为基础颜色的 alpha
    o_fragColor = finalColor;
}
