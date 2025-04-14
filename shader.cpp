#include "shader.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SimpleIni.h>

void Shader::init()
{
    // 从 INI 文件加载 shader 路径
    if (!loadShaderPathsFromIni("shader_config.ini"))
    {
        std::cerr << "Failed to load shader paths from INI file" << std::endl;
        // 使用默认路径作为备选
        vertexShaderPaths["normal"] = "shaders/vertex/normal.vert";
        vertexShaderPaths["wave"] = "shaders/vertex/wave.vert";
        vertexShaderPaths["breathing"] = "shaders/vertex/breathing.vert";

        fragmentShaderPaths["normal"] = "shaders/fragment/normal.frag";
        fragmentShaderPaths["pulse"] = "shaders/fragment/pulse.frag";
        fragmentShaderPaths["rainbow"] = "shaders/fragment/rainbow.frag";
    }

    // 初始化 shader 程序
    for (const auto &vShader : vertexShaderPaths)
    {
        for (const auto &fShader : fragmentShaderPaths)
        {
            std::string name = vShader.first + "_" + fShader.first;
            shaderPrograms[name] = createShader(vShader.second, fShader.second);
            if (shaderPrograms[name] == 0)
            {
                std::cerr << "Failed to create shader program: " << name << std::endl;
                return;
            }
        }
    }

    // 设置默认 shader 程序
    currentProgram = shaderPrograms["normal_normal"];
}

void Shader::cleanup()
{
    for (auto &program : shaderPrograms)
    {
        glDeleteProgram(program.second);
    }
    shaderPrograms.clear();
    currentProgram = 0;
}

void Shader::use()
{
    if (currentProgram != 0)
    {
        glUseProgram(currentProgram);
    }
}

void Shader::setUniform(const std::string &name, const glm::mat4 &value)
{
    if (currentProgram != 0)
    {
        GLint location = glGetUniformLocation(currentProgram, name.c_str());
        if (location != -1)
        {
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
        }
    }
}

void Shader::setUniform(const std::string &name, const glm::vec3 &value)
{
    if (currentProgram != 0)
    {
        GLint location = glGetUniformLocation(currentProgram, name.c_str());
        if (location != -1)
        {
            glUniform3fv(location, 1, glm::value_ptr(value));
        }
    }
}

void Shader::setUniform(const std::string &name, float value)
{
    if (currentProgram != 0)
    {
        GLint location = glGetUniformLocation(currentProgram, name.c_str());
        if (location != -1)
        {
            glUniform1f(location, value);
        }
    }
}

void Shader::setUniform(const std::string &name, int value)
{
    if (currentProgram != 0)
    {
        GLint location = glGetUniformLocation(currentProgram, name.c_str());
        if (location != -1)
        {
            glUniform1i(location, value);
        }
    }
}

void Shader::setUniform(const std::string &name, bool value)
{
    if (currentProgram != 0)
    {
        GLint location = glGetUniformLocation(currentProgram, name.c_str());
        if (location != -1)
        {
            glUniform1i(location, value ? 1 : 0);
        }
    }
}

void Shader::setCurrentProgram(const std::string &name)
{
    auto it = shaderPrograms.find(name);
    if (it != shaderPrograms.end())
    {
        currentProgram = it->second;
        use();
    }
    else
    {
        std::cerr << "Shader program '" << name << "' not found" << std::endl;
    }
}

void Shader::useShaderProgram(int vertexShaderIndex, int fragmentShaderIndex)
{
    // 将索引转换为字符串键
    std::string vertexKey;
    std::string fragmentKey;

    switch (vertexShaderIndex)
    {
    case 0:
        vertexKey = "normal";
        break;
    case 1:
        vertexKey = "wave";
        break;
    case 2:
        vertexKey = "breathing";
        break;
    default:
        vertexKey = "normal";
        break;
    }

    switch (fragmentShaderIndex)
    {
    case 0:
        fragmentKey = "normal";
        break;
    case 1:
        fragmentKey = "pulse";
        break;
    case 2:
        fragmentKey = "rainbow";
        break;
    default:
        fragmentKey = "normal";
        break;
    }

    // 组合成shader程序名称
    std::string programName = vertexKey + "_" + fragmentKey;

    // 设置当前shader程序
    setCurrentProgram(programName);
}

GLuint Shader::createShader(const std::string &vertexPath, const std::string &fragmentPath)
{
    std::string vertexCode = loadShaderSource(vertexPath);
    std::string fragmentCode = loadShaderSource(fragmentPath);

    GLuint vertexShader = compileShader(vertexCode, GL_VERTEX_SHADER);
    GLuint fragmentShader = compileShader(fragmentCode, GL_FRAGMENT_SHADER);

    GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetProgramInfoLog(program, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n"
                  << infoLog << std::endl;
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

bool Shader::loadShaderPathsFromIni(const std::string &filename)
{
    CSimpleIniA ini;
    SI_Error rc = ini.LoadFile(filename.c_str());
    if (rc < 0)
    {
        std::cerr << "Failed to load INI file: " << filename << std::endl;
        return false;
    }

    // 加载 shader 目录
    std::string vertexDir = ini.GetValue("ShaderPaths", "vertex_shaders_dir", "");
    std::string fragmentDir = ini.GetValue("ShaderPaths", "fragment_shaders_dir", "");

    // 加载顶点 shader 路径
    const char *vertexShaders[] = {"normal", "wave", "breathing"};
    for (int i = 0; i < 3; ++i)
    {
        std::string path = vertexDir + "/" + ini.GetValue("VertexShaders", vertexShaders[i], "");
        vertexShaderPaths[vertexShaders[i]] = path;
    }

    // 加载片段 shader 路径
    const char *fragmentShaders[] = {"normal", "pulse", "rainbow"};
    for (int i = 0; i < 3; ++i)
    {
        std::string path = fragmentDir + "/" + ini.GetValue("FragmentShaders", fragmentShaders[i], "");
        fragmentShaderPaths[fragmentShaders[i]] = path;
    }

    return true;
}

GLuint Shader::compileShader(const std::string &source, GLenum type)
{
    GLuint shader = glCreateShader(type);
    const char *src = source.c_str();
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "ERROR::SHADER::COMPILATION_FAILED\n"
                  << infoLog << std::endl;
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

std::string Shader::loadShaderSource(const std::string &path)
{
    std::string code;
    std::ifstream shaderFile;
    shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

    try
    {
        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        code = shaderStream.str();
    }
    catch (std::ifstream::failure &e)
    {
        std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << path << std::endl;
    }

    return code;
}