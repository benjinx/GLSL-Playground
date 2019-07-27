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

    void SaveShaderProgramAsBinary();
    void LoadShaderProgramAsBinary();
    void LoadShaderProgramAsSPRIV();

    void SendTriangleData();
    void GetActiveVertexInputAttribs();
    void GetActiveUniformVariables();

    void SendBlobData();

    void CreateShaderProgramViaPipeline1();
    void CreateShaderProgramViaPipeline2();

    void Render();
    void RenderPipelines(int width, int height);

    // Helper functions
    const char* GetTypeString(GLenum type);

private:
    GLuint _mVaoHandle;
    GLuint _mProgramHandle;

    // Pipeline Vars
    GLuint _mPrograms[3];
    GLuint _mPipelines[2];
};

#endif // SHADER_HPP