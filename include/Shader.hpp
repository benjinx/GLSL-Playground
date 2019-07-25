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

    void SaveShaderProgramAsBinary(GLuint programHandle);
    void LoadShaderProgramAsBinary();
    void LoadShaderProgramAsSPRIV();

    void SendTriangleData();
    void GetActiveVertexInputAttribs(GLuint programHandle);

    void Render();

    // Helper functions
    const char* GetTypeString(GLenum type);

private:
    GLuint _mVaoHandle;
    GLuint _mProgramHandle;
};

#endif // SHADER_HPP