shader-module vertex
  vec3 in in_position
  vec3 in in_normal
  vec3 out pass_normal

  std140 uniform-block matrices
  mat4 matrices projection
  mat4 matrices view

  : main ( -- )
    projection view * in_position expand-vec3 1.0 vec4 *
    !gl_Position

    in_normal abs
    !pass_normal
  ;

end-module

shader-module fragment
  vec3 in pass_normal
  vec4 out color
  : main pass_normal expand-vec3 1.0 vec4 !color ;
end-module

program-module primary
  vertex vertex-shader
  fragment fragment-shader
end-module

buffer-module arrays
  primary shader-program

  packed-buffer-begin
    vec3 shader-program:in_position ,
    vec3 shader-program:in_normal ,
  end-buffer

  create-vao vao
end-module

module main

data vertices
  0.0 1.0 0.0 vec3 ,
  0.0 0.0 1.0 vec3 ,
  -1.0 -1.0 0.0 vec3 ,
  0.0 0.0 1.0 vec3 ,
  1.0 -1.0 0.0 vec3 ,
  0.0 0.0 1.0 vec3 ,
end-data

variable ( arrays ) buffer

: main ( -- )
  buffer 3 initialize-buffer

  buffer data-ptr vertices vertices:size copy
;

: draw-loop
  drawClear

  buffer 0 6 draw

  poll-events
  swap-buffers
  draw-loop
;

end-module
