(in-package #:undersea)

(defun render (program vbo vaos)
  (gl:use-program program)
  (gl:bind-buffer :array-buffer vbo)
  (gl:bind-vertex-array vaos)
  (glfw:poll-events)
  (gl:clear :color-buffer-bit)
  (gl:draw-arrays :triangles 0 3)
  (glfw:swap-buffers))

(defun make-shader (type source)
  (let ((shader (gl:create-shader type)))
    (gl:shader-source shader source)
    (gl:compile-shader shader) shader))

(defun link-shaders (&rest shaders)
  (unwind-protect (let ((program (gl:create-program)))
                    (iterate (for s in shaders)
                      (gl:attach-shader program s))
                    (gl:link-program program)
                    program)
    (iterate (for s in shaders)
      (gl:delete-shader s))))

(defun make-program ()
  (let ((vertex-source
          (3bgl-shaders:generate-stage :vertex 'undersea/shaders:vertex))
        (fragment-source
          (3bgl-shaders:generate-stage :fragment 'undersea/shaders:fragment)))
    (link-shaders
     (make-shader :vertex-shader vertex-source)
     (make-shader :fragment-shader fragment-source))))

(defun start ()
  (setf 3bgl-shaders:*default-version* "460")
  (glfw:with-init-window (:width 800
                          :height 600
                          :title "Under Sea"
                          :context-version-major 4
                          :context-version-minor 6
                          :opengl-profile :opengl-core-profile)
    (gl:clear-color 0 0 0 1)
    (multiple-value-bind (buffer pointer)
        (make-buffer (* (cffi:foreign-type-size :float) 4 3) :array-buffer)
      (iterate
        (for v in '(0.0 1.0 0.0 1.0
                    -1.0 -1.0 0.0 1.0
                    1.0 -1.0 0.0 1.0))
        (for i from 0)
        (setf (cffi:mem-aref pointer :float i) v))
      (let ((vao (gl:create-vertex-array)))
        (gl:bind-vertex-array vao)
        (gl:vertex-attrib-pointer 0 4 :float nil 0 0)
        (gl:enable-vertex-attrib-array 0)
        (loop until (glfw:window-should-close-p)
              do (render (make-program)
                         buffer
                         vao))))))

(defconstant +persistent-flags+
  (reduce #'logior
          (mapcar (lambda (s)
                    (cffi:foreign-enum-value'%gl::mapbufferusagemask s))
                  '(:map-write-bit :map-persistent-bit :map-coherent-bit))))

(defun make-buffer (size target)
  (let ((buffer (gl:gen-buffer)))
    (gl:bind-buffer target buffer)
    (%gl:buffer-storage target size (cffi:null-pointer) +persistent-flags+)
    (values
     buffer
     (%gl:map-buffer-range target 0 size +persistent-flags+))))
