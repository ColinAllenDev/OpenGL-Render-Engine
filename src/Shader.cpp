#include "Shader.h"

Shader::Shader(const char* path) {
    ShaderSource sSource = LoadShader(path);
    const char* vShaderCode = sSource.VertexSource.c_str();
    const char* fShaderCode = sSource.FragmentSource.c_str();

    CompileShader(vShaderCode, fShaderCode);
}

Shader::Shader(const char* vPath, const char* fPath) {
    ShaderSource sSource = LoadShader(vPath, fPath);
    const char* vShaderCode = sSource.VertexSource.c_str();
    const char* fShaderCode = sSource.FragmentSource.c_str();

    CompileShader(vShaderCode, fShaderCode);
}

void Shader::Use() {
    glUseProgram(ID);
}

void Shader::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
};
void Shader::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
};
void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
};

void Shader::SetVec2(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec2(const std::string &name, float x, float y) const {
    glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::SetVec3(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec3(const std::string &name, float x, float y, float z) const {
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::SetVec4(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::SetVec4(const std::string &name, float x, float y, float z, float w) const {
    glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::SetMat2(const std::string &name, const glm::mat2 &mat) const {
    glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat3(const std::string &name, const glm::mat3 &mat) const {
    glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

void Shader::SetMat4(const std::string &name, const glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

ShaderSource Shader::LoadShader(const char *path) {
    /* Parse Shader */
    enum class ShaderType {NONE = -1, VERTEX = 0, FRAGMENT = 1};
    std::ifstream stream(path);
    std::stringstream  ss[2];
    std::string line;
    ShaderType type = ShaderType::NONE;

    /* Handle Exceptions */
    stream.exceptions(std::ifstream::badbit);

    try {
        while(getline(stream, line)) {
            if(line.find("#SHADER") != std::string::npos) {
                if (line.find("::VERTEX") != std::string::npos)
                    type = ShaderType::VERTEX;
                else if (line.find("::FRAGMENT") != std::string::npos)
                    type = ShaderType::FRAGMENT;
            } else {
                ss[(int)type] << line << '\n';
            }
        }

    } catch (std::ifstream::failure& e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
        std::cerr << "FILE: " << path << std::endl;
        std::cerr << e.what() << "\n" << std::endl;
    }

    return {ss[0].str(), ss[1].str()};
}

ShaderSource Shader::LoadShader(const char *vPath, const char* fPath) {
    std::string vShaderCode;
    std::string fShaderCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;

    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try {
        vShaderFile.open(vPath);
        fShaderFile.open(fPath);
        std::stringstream vStream, fStream;
        vStream << vShaderFile.rdbuf();
        fStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vShaderCode = vStream.str();
        fShaderCode = fStream.str();
    } catch (std::ifstream::failure e) {
        std::cerr << "ERROR::SHADER::FILE_NOT_READ_SUCCESSFULLY" << std::endl;
        std::cerr << vPath << "\n" << fPath << std::endl;
        std::cerr << e.what() << std::endl;
    }
    return {vShaderCode.c_str(), fShaderCode.c_str()};
}

void Shader::CompileShader(const char* vShader, const char* fShader) {
    /* Compile Shaders */
    unsigned int vertex, fragment;

    /* Vertex Shader */
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShader, NULL);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, "VERTEX");

    /* Fragment Shader */
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShader, NULL);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, "FRAGMENT");

    /* Shader Program */
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, "PROGRAM");

    /* Delete the shaders as they're linked into our program now and no longer necessary */
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::CheckCompileErrors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if(type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}