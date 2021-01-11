#include "Shader.h"

#include <fstream>
#include <iostream>
#include <GL/glew.h>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

struct ShaderProgramSource {
    std::string vertex;
    std::string fragment;
};

Shader::Shader(const std::string& filePath) : filePath(filePath), rendererId(0) {
    ShaderProgramSource source = ParseShader(filePath);
    rendererId = CreateShader(source.vertex, source.fragment);
}

Shader::~Shader() {
    GLCall(glDeleteProgram(rendererId));
}

void Shader::Bind() const {
    GLCall(glUseProgram(rendererId));
}

void Shader::Unbind() const {
    GLCall(glUseProgram(0));
}

void Shader::SetUniform1f(const std::string& name, float value) {
    GLCall(glUniform1f(GetUniformLocation(name), value));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
    GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name) {
    if(uniformLocationCache.find(name) != uniformLocationCache.end())
        return uniformLocationCache[name];
	
    GLCall(int location = glGetUniformLocation(rendererId, name.c_str()));
    if(location == -1)
        std::cout << "Warning: uniform " << name << " dose not exist." << std::endl;
    
    uniformLocationCache[name] = location;

    return location;
}

ShaderProgramSource Shader::ParseShader(const std::string& filePath) {
    std::ifstream stream(filePath);

    enum class ShaderType {
        None = -1,
        Vertex = 0,
        Fragment = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::None;
    while(getline(stream, line)) {
        if(line.find("#shader") != std::string::npos) {
            if(line.find("vertex") != std::string::npos) {
                type = ShaderType::Vertex;
            } else if(line.find("fragment") != std::string::npos) {
                type = ShaderType::Fragment;
            }
        } else {
            ss[static_cast<int>(type)] << line << '\n';
        }
    }

    return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    //Todo: Add error handling
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        //Get the length of the error message
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

        //Get the error message
        //char message[length]; //Length must be const so we need to hack the memory to be created on the stack
        char* message = static_cast<char*>(alloca(length * sizeof(char)));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader: " << message << std::endl;

        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    //The shader has already been linked to a program so we dont need them any more
    //Deletes the intermediate .obj files after the program has been built
    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}
