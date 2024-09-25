#version 450

out vec4 fragColor;

in vec3 v_texcoord;

uniform samplerCube u_skybox;

void main() {
    fragColor = texture(u_skybox, v_texcoord);
}