#include <Chapter3/ShaderC3.hpp>

#include <Utils.hpp>

#include <fstream>

namespace GLSLShaderInfo {
    std::map<std::string, GLSLShaderType> extensions = {
    { ".vs",         VERTEX },
    { ".vert",       VERTEX },
    { "_vert.glsl",  VERTEX },
    { ".vert.glsl",  VERTEX },
    { ".gs",         GEOMETRY },
    { ".geom",       GEOMETRY },
    { "_geom.glsl",  GEOMETRY },
    { ".geom.glsl",  GEOMETRY },
    { ".tcs",        TESS_CONTROL },
    { ".tcs.glsl",   TESS_CONTROL },
    { ".tes",        TESS_EVALUATION },
    { ".tes.glsl",   TESS_EVALUATION },
    { ".fs",         FRAGMENT },
    { ".frag",       FRAGMENT },
    { "_frag.glsl",  FRAGMENT },
    { ".frag.glsl",  FRAGMENT },
    { ".cs",         COMPUTE },
    { ".cs.glsl",    COMPUTE }
    };
}

ShaderC3::ShaderC3() : _mHandle(0), _mLinked(false) { }

ShaderC3::~ShaderC3()
{
    if (_mHandle == 0) return;
    DetachAndDeleteShaderObjects();

    // Delete the program
    glDeleteProgram(_mHandle);
}

void ShaderC3::DetachAndDeleteShaderObjects()
{
    // Detach and delete the shader objects (if they are not already removed)
    GLint numShaders = 0;
    glGetProgramiv(_mHandle, GL_ATTACHED_SHADERS, &numShaders);
    std::vector<GLuint> shaderNames(numShaders);
    glGetAttachedShaders(_mHandle, numShaders, NULL, shaderNames.data());
    for (GLuint shader : shaderNames) {
        glDetachShader(_mHandle, shader);
        glDeleteShader(shader);
    }
}

std::string ShaderC3::GetExtension(const std::string & filename) {
    std::string nameStr(filename);

    size_t dotLoc = nameStr.find_last_of('.');
    if (dotLoc != std::string::npos) {
        std::string ext = nameStr.substr(dotLoc);
        if (ext == ".glsl") {
            size_t loc = nameStr.find_last_of('.', dotLoc - 1);
            if (loc == std::string::npos) {
                loc = nameStr.find_last_of('_', dotLoc - 1);
            }
            if (loc != std::string::npos) {
                return nameStr.substr(loc);
            }
        }
        else
        {
            return ext;
        }
    }

    return "";
}

void ShaderC3::CompileShader(const std::string & filename)
{
    CheckExtension(filename);
}

void ShaderC3::CheckExtension(const std::string & filename)
{
    // Check the filenames extension to determine the shader type
    std::string ext = GetExtension(filename);

    GLSLShaderType type = VERTEX;
    auto it = GLSLShaderInfo::extensions.find(ext);
    if (it != GLSLShaderInfo::extensions.end())
    {
        type = it->second;
    }
    else
    {
        std::string msg = "Unrecognized Extension: " + ext;
        throw GLSLProgramException(msg);
    }

    // Pass the discovered shader type along
    LoadShaderAsString(filename, type);
}

void ShaderC3::LoadShaderAsString(const std::string & filename, GLSLShaderType type)
{
    const auto& paths = Utils::GetResourcePaths();

    printf("Loading: %s\n", filename.c_str());

    std::ifstream shaderFile;
    bool loaded = false;

    for (auto& p : paths)
    {
        std::string fullFilename = p + "/shaders/Chapter3/" + filename;

        shaderFile.open(fullFilename);

        if (shaderFile.is_open())
        {
            printf("Loaded: %s\n", filename.c_str());
            loaded = true;
            break;
        }
    }

    // Check if file was loaded
    if (!loaded)
    {
        std::string msg = "Failed to load: " + filename;
        throw GLSLProgramException(msg);
    }

    // Read in the file
    std::string shaderString((std::istreambuf_iterator<char>(shaderFile)),
                              std::istreambuf_iterator<char>());
    
    // Compile the shader
    CompileShader(shaderString, type, filename);
}

void ShaderC3::CompileShader(const std::string & source, GLSLShaderType type, const std::string & filename)
{
    if (_mHandle <= 0)
    {
        _mHandle = glCreateProgram();
        if (_mHandle == 0)
        {
            std::string msg = "Unable to create shader program.";
            throw GLSLProgramException(msg);
        }
    }

    // Create shader handle
    GLuint shaderHandle = glCreateShader(type);

    // Add the shader code to the handle
    const char * shaderCode = source.c_str();
    glShaderSource(shaderHandle, 1, &shaderCode, NULL);

    // Compile the shader
    glCompileShader(shaderHandle);

    // Check for errors
    int result;
    glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        // Compile Failed, Get log
        std::string msg;
        if (filename.c_str())
        {
            msg = std::string(filename) + ": Shader compliation failed.\n";
        }
        else
        {
            msg = "Shader compilation failed.\n";
        }

        int length = 0;
        glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);
        if (length > 0) {
            std::string log(length, ' ');
            int written = 0;
            glGetShaderInfoLog(shaderHandle, length, &written, &log[0]);
            msg += log;
        }

        throw GLSLProgramException(msg);
    }
    else
    {
        // Compile succeeded, attach shader
        glAttachShader(_mHandle, shaderHandle);
    }
}

void ShaderC3::Link()
{
    if (_mLinked)
    {
        return;
    }

    if (_mHandle <= 0)
    {
        throw GLSLProgramException("Program has not been compiled.");
    }

    glLinkProgram(_mHandle);

    int status = 0;
    std::string errString;
    glGetProgramiv(_mHandle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        // Store log
        int length = 0;
        glGetProgramiv(_mHandle, GL_INFO_LOG_LENGTH, &length);
        errString += "Program link failed:\n";
        if (length > 0)
        {
            std::string log(length, ' ');
            int written = 0;
            glGetProgramInfoLog(_mHandle, length, &written, &log[0]);
            errString += log;
        }
    }
    else
    {
        FindUniformLocations();
        _mLinked = true;
    }

    DetachAndDeleteShaderObjects();

    if (status == GL_FALSE)
    {
        throw GLSLProgramException(errString);
    }
}

void ShaderC3::FindUniformLocations()
{
    _mUniformLocations.clear();

    GLint numUniforms = 0;

    glGetProgramInterfaceiv(_mHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

    GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

    for (GLint i = 0; i < numUniforms; i++)
    {
        GLint results[4];
        glGetProgramResourceiv(_mHandle, GL_UNIFORM, i, 4, properties, 4, NULL, results);

        if (results[3] != -1) continue; // Skip uniforms in blocks
        GLint nameBufSize = results[0] + 1;
        char * name = new char[nameBufSize];
        glGetProgramResourceName(_mHandle, GL_UNIFORM, i, nameBufSize, NULL, name);
        _mUniformLocations[name] = results[2];
        delete[] name;
    }
}

void ShaderC3::Validate()
{
    if (!IsLinked())
    {
        throw GLSLProgramException("Program is not linked.");
    }

    GLint status;
    glValidateProgram(_mHandle);
    glGetProgramiv(_mHandle, GL_VALIDATE_STATUS, &status);

    if (status == GL_FALSE)
    {
        int length = 0;
        std::string logString;

        glGetProgramiv(_mHandle, GL_INFO_LOG_LENGTH, &length);

        if (length > 0)
        {
            char * c_log = new char[length];
            int written = 0;
            glGetProgramInfoLog(_mHandle, length, &written, c_log);
            logString = c_log;
            delete[] c_log;
        }

        throw GLSLProgramException(std::string("Program failed to validate\n") + logString);
    }
}

void ShaderC3::Use()
{
    if (_mHandle <= 0 || (!_mLinked))
    {
        throw GLSLProgramException("Shader has not been linked.");
    }

    glUseProgram(_mHandle);
}

void ShaderC3::SetUniform(const char * name, float x, float y, float z) {
    GLint loc = GetUniformLocation(name);
    glUniform3f(loc, x, y, z);
}

void ShaderC3::SetUniform(const char * name, const glm::vec3 &v) {
    this->SetUniform(name, v.x, v.y, v.z);
}

void ShaderC3::SetUniform(const char * name, const glm::vec4 &v) {
    GLint loc = GetUniformLocation(name);
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void ShaderC3::SetUniform(const char * name, const glm::vec2 &v) {
    GLint loc = GetUniformLocation(name);
    glUniform2f(loc, v.x, v.y);
}

void ShaderC3::SetUniform(const char * name, const glm::mat4 &m) {
    GLint loc = GetUniformLocation(name);
    glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void ShaderC3::SetUniform(const char * name, const glm::mat3 &m) {
    GLint loc = GetUniformLocation(name);
    glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void ShaderC3::SetUniform(const char * name, float val) {
    GLint loc = GetUniformLocation(name);
    glUniform1f(loc, val);
}

void ShaderC3::SetUniform(const char * name, int val) {
    GLint loc = GetUniformLocation(name);
    glUniform1i(loc, val);
}

void ShaderC3::SetUniform(const char * name, GLuint val) {
    GLint loc = GetUniformLocation(name);
    glUniform1ui(loc, val);
}

void ShaderC3::SetUniform(const char * name, bool val) {
    int loc = GetUniformLocation(name);
    glUniform1i(loc, val);
}