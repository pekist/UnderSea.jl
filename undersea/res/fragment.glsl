#version 460 core

in vec3 pass_normal;

out vec4 color;

layout (location = 3) uniform sampler2D textureUnit;

void main(){
     color = vec4(pass_normal, 1.0);
}
