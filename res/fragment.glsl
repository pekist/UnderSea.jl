#version 460 core

in vec2 out_texture;
out vec4 color;

layout (location = 3) uniform sampler2D textureUnit;

void main(){
    color = texture(textureUnit, out_texture);
}
