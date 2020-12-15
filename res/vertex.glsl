#version 460 core

layout (location = 0) in vec2 position;

layout (std140, binding = 2) uniform Matrices {
    // offset: 0
    mat4 projection;
    // offset: 64
    mat4 view;
};

void main(){
  gl_Position = projection * view * vec4(position.x, position.y, -10.0f, 1.0f);
}
