(defpackage #:undersea
  (:use #:cl #:alexandria #:iterate))
(defpackage #:undersea/shaders
  (:use #:3bgl-glsl/cl)
  (:export vertex
           fragment))
