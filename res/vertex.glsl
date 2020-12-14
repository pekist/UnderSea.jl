#version 460 core

layout (location = 0) in vec2 position;

void main(){
  gl_Position = vec4(position.y, position.x, 0, 1);
}
