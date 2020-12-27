#version 460 core

layout (location = 0) in vec3 in_position;
layout (location = 1) in vec3 in_normal;

layout (std140, binding = 2) uniform Matrices {
    // offset: 0
    mat4 projection;
    // offset: 64
    mat4 view;
};

out vec3 pass_normal;

void main(){
  gl_Position = projection * view * vec4(in_position, 1.0);
  pass_normal = abs(in_normal);
}
