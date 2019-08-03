#ifndef SHADER_C4_HPP
#define SHADER_C4_HPP

#include <OpenGL.hpp>

#include <map>
#include <string>
#include <vector>

class GLSLProgramException : public std::runtime_error {
public:
    GLSLProgramException(const std::string & msg) : std::runtime_error(msg) {}
};

enum GLSLShaderTypeC4 {
    VERTEX = GL_VERTEX_SHADER,
    FRAGMENT = GL_FRAGMENT_SHADER,
    GEOMETRY = GL_GEOMETRY_SHADER,
    TESS_CONTROL = GL_TESS_CONTROL_SHADER,
    TESS_EVALUATION = GL_TESS_EVALUATION_SHADER,
    COMPUTE = GL_COMPUTE_SHADER
};

class ShaderC4 {
public:
    ShaderC4();
    ~ShaderC4();

    void CompileShader(const std::string & filename);
    
    void Link();
    void Validate();
    void Use();

    int GetHandle() { return _mHandle; }
    bool IsLinked() { return _mLinked; }

    void SetUniform(const char * name, float x, float y, float z);
    void SetUniform(const char * name, const glm::vec2 & v);
    void SetUniform(const char * name, const glm::vec3 & v);
    void SetUniform(const char * name, const glm::vec4 & v);
    void SetUniform(const char * name, const glm::mat3 & m);
    void SetUniform(const char * name, const glm::mat4 & m);
    void SetUniform(const char * name, float val);
    void SetUniform(const char * name, int val);
    void SetUniform(const char * name, bool val);
    void SetUniform(const char * name, GLuint val);

    void FindUniformLocations();

private:
    int _mHandle;
    int _mLinked;
    std::map<std::string, int> _mUniformLocations;

    // These are a chain stemming from the public CompileShader function.
    void CheckExtension(const std::string & filename);
    void LoadShaderAsString(const std::string & filename, GLSLShaderTypeC4 type);
    void CompileShader(const std::string & source, GLSLShaderTypeC4 type, const std::string & filename = nullptr);

    inline GLint GetUniformLocation(const char * name);
    void DetachAndDeleteShaderObjects();
    std::string GetExtension(const std::string & filename);
};

int ShaderC4::GetUniformLocation(const char * name)
{
    auto pos = _mUniformLocations.find(name);

    if (pos == _mUniformLocations.end())
    {
        GLint loc = glGetUniformLocation(_mHandle, name);
        _mUniformLocations[name] = loc;
        return loc;
    }

    return pos->second;
}

#endif // SHADER_C3_HPP
