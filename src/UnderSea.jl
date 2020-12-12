module UnderSea

using GLFW
using ModernGL

include("shader.jl")

function ModernGL.getprocaddress(name::String)
    GLFW.GetProcAddress(name)
end

triangle_vertices =
    [Float32(0.0), Float32(1.0), Float32(-1.0), Float32(-1.0), Float32(1.0), Float32(-1.0)]

function prepare_shaders()
    vertex_shader = open("../res/vertex.glsl", "r") do vertex_file
        compile_shader(GL_VERTEX_SHADER, vertex_file)
    end
    fragment_shader = open("../res/fragment.glsl", "r") do fragment_file
        compile_shader(GL_FRAGMENT_SHADER, fragment_file)
    end

    if vertex_shader === nothing || fragment_shader === nothing
        for shader in [vertex_shader, fragment_shader]
            shader === nothing || glDeleteShader(shader)
        end
        return nothing
    end

    program = link_shaders([vertex_shader, fragment_shader])
    glDeleteShader(vertex_shader)
    glDeleteShader(fragment_shader)
    program
end

function prepare_buffers()
    vbos = Vector{UInt32}(undef, 1)
    vaos = Vector{UInt32}(undef, 1)
    glGenBuffers(1, vbos)
    glGenVertexArrays(1, vaos)
    vao = vaos[1]
    vbo = vbos[1]
    glBindVertexArray(vao)
    glBindBuffer(GL_ARRAY_BUFFER, vbo)
    glBufferData(
        GL_ARRAY_BUFFER,
        length(triangle_vertices) * 4,
        triangle_vertices,
        GL_STATIC_DRAW,
    )
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), C_NULL)
    glEnableVertexAttribArray(0)

    vbo, vao
end

function main()
    GLFW.WindowHint(GLFW.CONTEXT_VERSION_MAJOR, 4)
    GLFW.WindowHint(GLFW.CONTEXT_VERSION_MINOR, 6)
    window = GLFW.CreateWindow(640, 480, "GLFW")
    GLFW.MakeContextCurrent(window)
    vbo = 0
    vao = 0
    try
        glClearColor(0, 0, 0, 1)
        vbo, vao = prepare_buffers()
        program = prepare_shaders()
        glUseProgram(program)
        while !GLFW.WindowShouldClose(window)
            glClear(GL_COLOR_BUFFER_BIT)
            glDrawArrays(GL_TRIANGLES, 0, 3)
            GLFW.SwapBuffers(window)
            GLFW.PollEvents()
        end


    finally
        vbo == 0 || glDeleteBuffers(1, [vbo])
        vao == 0 || glDeleteVertexArrays(1, [vao])
        GLFW.DestroyWindow(window)
    end
end
end
