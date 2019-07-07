#ifndef SHADER_HPP
#define SHADER_HPP

#include <string>

#include <OpenGL.hpp>

class Shader {
public:
    Shader();
    ~Shader();

    void Load(std::string vertFile, std::string fragFile);

    std::string LoadShaderAsString(std::string fileName);
    void PrintVersions();
    void PrintExtensions();
};

#endif // SHADER_HPP