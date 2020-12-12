function compile_shader(shader_type, string::String)
    shader = glCreateShader(shader_type)
    glShaderSource(shader, 1, [string], [length(string)])
    glCompileShader(shader)
    status = Vector{Int32}(undef, 1)
    glGetShaderiv(shader, GL_COMPILE_STATUS, status)
    if status[1] == 0
        len = Vector{Int32}(undef, 1)
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, len)
        str = Vector{UInt8}(undef, len[1])
        glGetShaderInfoLog(shader, len[1], C_NULL, str)
        for c in str
            print(Char(c))
        end
        glDeleteShader(shader)
        nothing
    else
        shader
    end
end

function compile_shader(shader_type, file::IOStream)
    shader_source = read(file, String)
    compile_shader(shader_type, shader_source)
end

function link_shaders(shaders)
    program = glCreateProgram()
    for s in shaders
        glAttachShader(program, s)
    end

    glLinkProgram(program)

    status = Vector{Int32}(undef, 1)
    glGetProgramiv(program, GL_LINK_STATUS, status)
    if status[1] == 0
        len = Vector{Int32}(undef, 1)
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, status)
        info_log = Vector{UInt32}(undef, len[1])
        glGetProgramInfoLog(program, len[1], C_NULL, info_log)
        for c in info_log
            print(Char(c))
        end

        glDeleteProgram(program)
        return nothing
    end

    return program
end
