#include <Shader.hpp>

#include <fstream>
#include <iostream>

#include <Utils.hpp>

Shader::Shader()
{
}

Shader::~Shader()
{

}

void Shader::Load(std::string vertFile, std::string fragFile)
{
    //////////////////////////////
    // Read in our shaders
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);
    if (vertShader == 0)
    {
        std::cerr << "Error creating vertex shader.\n";
        exit(EXIT_FAILURE);
    }

    std::string shaderCode = LoadShaderAsString(vertFile);
    const GLchar* vertexArray[] = { shaderCode.c_str() };
    glShaderSource(vertShader, 1, vertexArray, NULL);

    glCompileShader(vertShader);

    GLint result;
    glGetShaderiv(vertShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        std::cerr << "Vertex shader compilation failed!\n";


        // Get and print the info log
        GLint logLen;
        glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0)
        {
            std::string log(logLen, ' ');
            GLsizei written;
            glGetShaderInfoLog(vertShader, logLen, &written, &log[0]);
            std::cerr << "Shader log: \n" << log;
        }
    }

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);
    if (fragShader == 0)
    {
        std::cerr << "Error creating fragment shader.\n";
        exit(EXIT_FAILURE);
    }

    shaderCode = LoadShaderAsString(fragFile);
    const GLchar* fragmentArray[] = { shaderCode.c_str() };
    glShaderSource(fragShader, 1, fragmentArray, NULL);

    glCompileShader(fragShader);

    glGetShaderiv(fragShader, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        std::cerr << "Fragment shader compilation failed!\n";


        // Get and print the info log
        GLint logLen;
        glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0)
        {
            std::string log(logLen, ' ');
            GLsizei written;
            glGetShaderInfoLog(fragShader, logLen, &written, &log[0]);
            std::cerr << "Shader log: \n" << log;
        }
    }


    // Link our shaders
    GLuint programHandle = glCreateProgram();
    if (programHandle == 0)
    {
        std::cerr << "Error Creating program object.\n";
        exit(EXIT_FAILURE);
    }

    glAttachShader(programHandle, vertShader);
    glAttachShader(programHandle, fragShader);

    glLinkProgram(programHandle);

    GLint status;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cerr << "Failed to link shader program.\n";
        GLint logLen;
        glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            std::string log(logLen, ' ');
            GLsizei written;
            glGetProgramInfoLog(programHandle, logLen, &written, &log[0]);
            std::cerr << "Program log: \n" << log;
        }
    }
    else
    {
        std::cout << "Shader Program Linked!\n";
        glUseProgram(programHandle);
    }

    SaveShaderProgramAsBinary(programHandle);

    // Make sure we delete it
    glDetachShader(programHandle, vertShader);
    glDetachShader(programHandle, fragShader);
    glDeleteShader(fragShader);
    glDeleteShader(vertShader);

    //////////////////////////////
}

void Shader::SaveShaderProgramAsBinary(GLuint programHandle)
{
    /// Binary Shader Saving
    GLint formats = 0;
    glGetIntegerv(GL_NUM_PROGRAM_BINARY_FORMATS, &formats);
    if (formats < 1) {
        std::cerr << "Driver does not support any binary formats.\n";
        exit(EXIT_FAILURE);
    }

    // Get binary length
    GLint length = 0;
    glGetProgramiv(programHandle, GL_PROGRAM_BINARY_LENGTH, &length);

    // Retrieve the binary code
    std::vector<GLubyte> buffer(length);
    GLenum format = 0;

    glGetProgramBinary(programHandle, length, NULL, &format, buffer.data());

    // Write the binary to a file.
    std::string fName("Shader.bin");
    std::cout << "Writing to " << fName << ", binary format = " << format << std::endl;
    std::ofstream out(fName.c_str(), std::ios::binary);
    out.write(reinterpret_cast<char *>(buffer.data()), length);
    out.close();
}

void Shader::LoadShaderProgramAsBinary()
{
    GLuint programHandle = glCreateProgram();

    // Need to find a way to figure out the format :thinking:
    GLenum format = 36385;

    // Load binary from file
    std::ifstream inputStream("Shader.bin", std::ios::binary);
    std::istreambuf_iterator<char> startIt(inputStream), endIt;
    std::vector<char> buffer(startIt, endIt); // Load file
    inputStream.close();

    // Install shader binary
    glProgramBinary(programHandle, format, buffer.data(), buffer.size());

    // Check for success/failure
    GLint status;
    glGetProgramiv(programHandle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cerr << "Failed to link shader program.\n";
        GLint logLen;
        glGetProgramiv(programHandle, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            std::string log(logLen, ' ');
            GLsizei written;
            glGetProgramInfoLog(programHandle, logLen, &written, &log[0]);
            std::cerr << "Program log: \n" << log;
        }
    }
    else
    {
        std::cout << "Shader Program Linked!\n";
        glUseProgram(programHandle);
    }
}

std::string Shader::LoadShaderAsString(std::string fileName)
{
    const auto& paths = Utils::GetResourcePaths();

    std::cout << "Loading: " << fileName << "\n";

    std::ifstream shaderFile;
    bool loaded = false;

    for (auto& p : paths)
    {
        std::string fullFilename = p + "/shaders/" + fileName;

        shaderFile.open(fullFilename);

        if (shaderFile.is_open())
        {
            std::cout << "Loaded: " << fileName << "\n";
            loaded = true;
            break;
        }
    }

    if (!loaded)
    {
        std::cout << "Failed to load: " << fileName << "\n";
        std::string failedString;
        return failedString;
    }

    std::string shaderString((std::istreambuf_iterator<char>(shaderFile)),
        std::istreambuf_iterator<char>());

    return shaderString;

}


void Shader::PrintVersions()
{
    // Determining the GLSL and OpenGL version
    const GLubyte* renderer = glGetString(GL_RENDERER);
    const GLubyte* vendor = glGetString(GL_VENDOR);
    const GLubyte* version = glGetString(GL_VERSION);
    const GLubyte* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

    GLint major, minor;
    glGetIntegerv(GL_MAJOR_VERSION, &major);
    glGetIntegerv(GL_MINOR_VERSION, &minor);

    printf("GL Vendor               : %s\n", vendor);
    printf("GL Renderer             : %s\n", renderer);
    printf("GL Version (String)     : %s\n", version);
    printf("GL Version (Integer)    : %d.%d\n", major, minor);
    printf("GLSL Version            : %s\n", glslVersion);
    printf("\n");
}

void Shader::PrintExtensions()
{
    // Extensions
    GLint nExtensions;
    glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);

    for (int i = 0; i < nExtensions; i++)
        printf("%s\n", glGetStringi(GL_EXTENSIONS, i));
}