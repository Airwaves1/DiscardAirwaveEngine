#version 460

layout(location = 0) out vec4 o_fragColor;

layout(location = 0) in vec3 v_color;

void main() {
    o_fragColor = vec4(v_color, 1.0);
}