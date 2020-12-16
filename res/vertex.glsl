#version 460 core

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec2 in_texture;

layout (std140, binding = 2) uniform Matrices {
    // offset: 0
    mat4 projection;
    // offset: 64
    mat4 view;
};

out vec2 out_texture;

void main(){
  gl_Position = projection * view * in_position;
  out_texture = in_texture;
}
