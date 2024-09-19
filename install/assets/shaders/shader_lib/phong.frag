#version 460

#define USE_DIFFUSE_MAP
#define USE_SPECULAR_MAP
#define USE_NORMAL_MAP

layout(location = 0) out vec4 o_fragColor;

layout(location = 0) in vec3 v_normal;
layout(location = 1) in vec2 v_uv;
layout(location = 2) in vec3 v_worldPosition;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;

    sampler2D diffuseMap;
    sampler2D specularMap;
    sampler2D normalMap;
};

struct Light {
    int type;   // 0: point light, 1: directional light, 2: spot light
    vec3 position;
    vec3 direction;
    float intensity;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Material u_material;
uniform Light u_lights[16];    // 最多支持 16 个光源
uniform int u_lightCount;       // 光源数量

uniform vec3 u_cameraPosition;  // 相机位置

vec3 CalPointLight(vec3 normal, vec3 worldPosition, vec3 cameraPosition, Light light);

void main() {
    vec4 finalColor = vec4(vec3(0.), 1.0);

    for(int i = 0; i < u_lightCount; i++) {
        if(u_lights[i].type == 0) {
            finalColor.rgb += CalPointLight(v_normal, v_worldPosition, u_cameraPosition, u_lights[i]);
        }
    }

    // 输出最终颜色，alpha 值为基础颜色的 alpha
    o_fragColor = finalColor;
}

vec3 CalPointLight(vec3 normal, vec3 worldPosition, vec3 cameraPosition, Light light) {
    vec3 f = vec3(0.0);

    // 漫反射
    normal = normalize(normal);
    
    vec3 lightDir = normalize(worldPosition - light.position);
    
    float diff = max(dot(normal, -lightDir), 0.0);

    vec3 viewDir = normalize(worldPosition - cameraPosition);
    
    vec3 halfDir = normalize(-lightDir - viewDir);

    float flag = step(0.0, dot(-lightDir, normal));  // 解决背面光照问题

#ifdef USE_DIFFUSE_MAP

    vec3 diffuseColor = texture(u_material.diffuseMap, v_uv).rgb;
    vec3 specularColor = texture(u_material.specularMap, v_uv).rgb;

    vec3 ambient = light.ambient * diffuseColor;

    vec3 diffuse = light.diffuse * diff * diffuseColor;


    float spec = pow(max(dot(normal, halfDir), 0.0), u_material.shininess) * flag;

    vec3 specular = (light.specular * spec) * specularColor;

#else

    vec3 ambient = light.ambient * u_material.ambient;

    vec3 diffuse = (light.diffuse * diff) * u_material.diffuse;

    float spec = pow(max(dot(normal, halfDir), 0.0), u_material.shininess) * flag;
    vec3 specular = (light.specular * spec) * u_material.specular;


#endif
    f = ambient + diffuse + specular;
    return f;
}