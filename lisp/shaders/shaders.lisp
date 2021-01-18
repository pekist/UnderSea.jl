(in-package #:undersea/shaders)

(input position :vec4 :location 0)
(output out-color :vec4 :stage :fragment)

(defun vertex ()
  (setf gl-position position))

(defun fragment ()
  (setf out-color (vec4 1.0 1.0 1.0 1.0)))
