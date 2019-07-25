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
    _mProgramHandle = glCreateProgram();
    if (_mProgramHandle == 0)
    {
        std::cerr << "Error Creating program object.\n";
        exit(EXIT_FAILURE);
    }

    glAttachShader(_mProgramHandle, vertShader);
    glAttachShader(_mProgramHandle, fragShader);

    glLinkProgram(_mProgramHandle);

    GLint status;
    glGetProgramiv(_mProgramHandle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cerr << "Failed to link shader program.\n";
        GLint logLen;
        glGetProgramiv(_mProgramHandle, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            std::string log(logLen, ' ');
            GLsizei written;
            glGetProgramInfoLog(_mProgramHandle, logLen, &written, &log[0]);
            std::cerr << "Program log: \n" << log;
        }
    }
    else
    {
        std::cout << "Shader Program Linked!\n";
        glUseProgram(_mProgramHandle);
    }

    SaveShaderProgramAsBinary();
    
    // Make sure we delete it
    glDetachShader(_mProgramHandle, vertShader);
    glDetachShader(_mProgramHandle, fragShader);
    glDeleteShader(fragShader);
    glDeleteShader(vertShader);

    //////////////////////////////
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

void Shader::SaveShaderProgramAsBinary()
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
    glGetProgramiv(_mProgramHandle, GL_PROGRAM_BINARY_LENGTH, &length);

    // Retrieve the binary code
    std::vector<GLubyte> buffer(length);
    GLenum format = 0;

    glGetProgramBinary(_mProgramHandle, length, NULL, &format, buffer.data());

    // Write the binary to a file.
    std::string fName("Shader.bin");
    std::cout << "Writing to " << fName << ", binary format = " << format << std::endl;
    std::ofstream out(fName.c_str(), std::ios::binary);
    out.write(reinterpret_cast<char *>(buffer.data()), length);
    out.close();
}

void Shader::LoadShaderProgramAsBinary()
{
    _mProgramHandle = glCreateProgram();

    // Need to find a way to figure out the format :thinking:
    // Write to the file the format type first then load it here. - Steve
    GLenum format = 36385;

    // Load binary from file
    std::ifstream inputStream("Shader.bin", std::ios::binary);
    std::istreambuf_iterator<char> startIt(inputStream), endIt;
    std::vector<char> buffer(startIt, endIt); // Load file
    inputStream.close();

    // Install shader binary
    glProgramBinary(_mProgramHandle, format, buffer.data(), buffer.size());

    // Check for success/failure
    GLint status;
    glGetProgramiv(_mProgramHandle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cerr << "Failed to link shader program.\n";
        GLint logLen;
        glGetProgramiv(_mProgramHandle, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            std::string log(logLen, ' ');
            GLsizei written;
            glGetProgramInfoLog(_mProgramHandle, logLen, &written, &log[0]);
            std::cerr << "Program log: \n" << log;
        }
    }
    else
    {
        std::cout << "Shader Program Linked!\n";
        glUseProgram(_mProgramHandle);
    }
}

void Shader::LoadShaderProgramAsSPRIV()
{
    GLuint vertShader = glCreateShader(GL_VERTEX_SHADER);

    // Load the shader into a std::vector
    std::ifstream inStream("basic.vert.spv", std::ios::binary);
    std::istreambuf_iterator<char> startIt(inStream), endIt;
    std::vector<char> buffer(startIt, endIt);
    inStream.close();

    // Load using glShaderBinary
    glShaderBinary(1, &vertShader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, buffer.data(), buffer.size());

    // Specialize the shader (specify the entry point)
    glSpecializeShaderARB(vertShader, "main", 0, 0, 0);

    // Check for success/failure
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
    else
    {
        printf("Vertex Shader Loaded!\n");
    }

    GLuint fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Load the shader into a std::vector
    std::ifstream inStream2("basic.frag.spv", std::ios::binary);
    std::istreambuf_iterator<char> startIt2(inStream2), endIt2;
    std::vector<char> buffer2(startIt2, endIt2);
    inStream2.close();

    // Load using glShaderBinary
    glShaderBinary(1, &fragShader, GL_SHADER_BINARY_FORMAT_SPIR_V_ARB, buffer2.data(), buffer2.size());

    // Specialize the shader (specify the entry point)
    glSpecializeShaderARB(fragShader, "main", 0, 0, 0);

    // Check for success/failure
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
    else
    {
        printf("Fragment Shader Loaded!\n");
    }

    // Link our shaders
    _mProgramHandle = glCreateProgram();
    if (_mProgramHandle == 0)
    {
        std::cerr << "Error Creating program object.\n";
        exit(EXIT_FAILURE);
    }

    glAttachShader(_mProgramHandle, vertShader);
    glAttachShader(_mProgramHandle, fragShader);

    glLinkProgram(_mProgramHandle);

    GLint status;
    glGetProgramiv(_mProgramHandle, GL_LINK_STATUS, &status);
    if (status == GL_FALSE)
    {
        std::cerr << "Failed to link shader program.\n";
        GLint logLen;
        glGetProgramiv(_mProgramHandle, GL_INFO_LOG_LENGTH, &logLen);
        if (logLen > 0) {
            std::string log(logLen, ' ');
            GLsizei written;
            glGetProgramInfoLog(_mProgramHandle, logLen, &written, &log[0]);
            std::cerr << "Program log: \n" << log;
        }
    }
    else
    {
        std::cout << "Shader Program Linked!\n";
        glUseProgram(_mProgramHandle);
    }

    // Make sure we delete it
    glDetachShader(_mProgramHandle, vertShader);
    glDetachShader(_mProgramHandle, fragShader);
    glDeleteShader(fragShader);
    glDeleteShader(vertShader);

    //////////////////////////////
}

void Shader::SendTriangleData()
{
    // Create VAO for triangle
    //GLuint vaoHandle;

    // Position & Color data for our Triangle
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.0f, 0.8f, 0.0f
    };

    float colorData[] = {
        1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 1.0f
    };

    // Create and populate the buffer objects
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint colorBufferHandle = vboHandles[1];

    // Populate the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), positionData, GL_STATIC_DRAW);

    // Populate the color buffer
    glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), colorData, GL_STATIC_DRAW);

    // Create and set-up the vertex array object
    glGenVertexArrays(1, &_mVaoHandle);
    glBindVertexArray(_mVaoHandle);

    // Enable the vertex attribute arrays
    glEnableVertexAttribArray(0); // Vertex Position
    glEnableVertexAttribArray(1); // Vertex Color

    //// Map index 0 to the position buffer
    //glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    //glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    //// Map index 1 to the color buffer
    //glBindBuffer(GL_ARRAY_BUFFER, colorBufferHandle);
    //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindVertexBuffer(0, positionBufferHandle, 0, sizeof(GLfloat) * 3);
    glBindVertexBuffer(1, colorBufferHandle, 0, sizeof(GLfloat) * 3);

    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
    glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(1, 1);

    // Anything for rotation?
    float angle = glm::radians(180.0f);
    glm::mat4 rotationMatrix = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 0.0f, 1.0f));
    GLuint location = glGetUniformLocation(_mProgramHandle, "RotationMatrix");

    if (location >= 0) { // >= 0 meaning it's not -1, if it was -1 that would mean RotationMatrix did not exist/wasn't active.
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
    }
}

void Shader::GetActiveVertexInputAttribs()
{
    GLint numAttribs;
    glGetProgramInterfaceiv(_mProgramHandle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);

    GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

    std::cout << "Active Attributes:\n";
    for (int i = 0; i < numAttribs; i++)
    {
        GLint results[3];
        glGetProgramResourceiv(_mProgramHandle, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results);

        GLint nameBufSize = results[0] + 1;
        char* name = new char[nameBufSize];
        glGetProgramResourceName(_mProgramHandle, GL_PROGRAM_INPUT, i, nameBufSize, NULL, name);
        printf("%-5d %s (%s)\n", results[2], name, GetTypeString(results[1]));
        delete[] name;
    }
}

void Shader::GetActiveUniformVariables()
{
    GLint numUniforms = 0;
    glGetProgramInterfaceiv(_mProgramHandle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

    GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

    std::cout << "Active Uniforms:\n";

    for (int i = 0; i < numUniforms; i++)
    {
        GLint results[4];
        glGetProgramResourceiv(_mProgramHandle, GL_UNIFORM, i, 4, properties, 4, NULL, results);
        if (results[3] != -1)
            continue;   // Skip uniforms in blocks
        GLint nameBufSize = results[0] + 1;
        char* name = new char[nameBufSize];
        glGetProgramResourceName(_mProgramHandle, GL_UNIFORM, i, nameBufSize, NULL, name);
        printf("%-5d %s (%s)\n", results[2], name, GetTypeString(results[1]));
        delete[] name;
    }
}

void Shader::SendBlobData()
{
    // Create VAO for triangle
    //GLuint vaoHandle;

    // Position & Color data for our Triangle
    float positionData[] = {
        -0.8f, -0.8f, 0.0f,
        0.8f, -0.8f, 0.0f,
        0.8f,  0.8f, 0.0f,
        -0.8f, -0.8f, 0.0f,
        0.8f, 0.8f, 0.0f,
        -0.8f, 0.8f, 0.0f
    };

    float texCoordData[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f
    };

    // Create and populate the buffer objects
    GLuint vboHandles[2];
    glGenBuffers(2, vboHandles);
    GLuint positionBufferHandle = vboHandles[0];
    GLuint texCoordDataBufferHandle = vboHandles[1];

    // Populate the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 18 * sizeof(float), positionData, GL_STATIC_DRAW);

    // Populate the color buffer
    glBindBuffer(GL_ARRAY_BUFFER, texCoordDataBufferHandle);
    glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), texCoordData, GL_STATIC_DRAW);

    // Create and set-up the vertex array object
    glGenVertexArrays(1, &_mVaoHandle);
    glBindVertexArray(_mVaoHandle);

    // Enable the vertex attribute arrays
    glEnableVertexAttribArray(0); // Vertex Position
    glEnableVertexAttribArray(1); // Vertex Color

    // Map index 0 to the position buffer
    glBindBuffer(GL_ARRAY_BUFFER, positionBufferHandle);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    // Map index 1 to the TexCoord buffer
    glBindBuffer(GL_ARRAY_BUFFER, texCoordDataBufferHandle);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    GLuint blockIndex = glGetUniformBlockIndex(_mProgramHandle, "BlobSettings");

    GLint blockSize;
    glGetActiveUniformBlockiv(_mProgramHandle, blockIndex, GL_UNIFORM_BLOCK_DATA_SIZE, &blockSize);

    GLubyte* blockBuffer;
    blockBuffer = (GLubyte*)malloc(blockSize);

    const GLchar* names[] = { "InnerColor",
                              "OuterColor",
                              "RadiusInner",
                              "RadiusOuter"
    };

    GLuint indices[4];
    glGetUniformIndices(_mProgramHandle, 4, names, indices);

    GLint offset[4];
    glGetActiveUniformsiv(_mProgramHandle, 4, indices, GL_UNIFORM_OFFSET, offset);

    // Store data within the buffer at the appropriate offsets
    GLfloat outerColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    GLfloat innerColor[] = { 1.0f, 1.0f, 0.75f, 1.0f };
    GLfloat innerRadius = 0.25f,
            outerRadius = 0.45f;

    memcpy(blockBuffer + offset[0], innerColor, 4 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[1], outerColor, 4 * sizeof(GLfloat));
    memcpy(blockBuffer + offset[2], &innerRadius, sizeof(GLfloat));
    memcpy(blockBuffer + offset[3], &outerRadius, sizeof(GLfloat));

    GLuint uboHandle;
    glGenBuffers(1, &uboHandle);
    glBindBuffer(GL_UNIFORM_BUFFER, uboHandle);
    glBufferData(GL_UNIFORM_BUFFER, blockSize, blockBuffer, GL_DYNAMIC_DRAW);

    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uboHandle);
}

void Shader::Render()
{
    // Clear to render
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);



    // Render the triangle
    glBindVertexArray(_mVaoHandle);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

const char* Shader::GetTypeString(GLenum type) {
    switch (type) {
    case GL_FLOAT:
        return "float";
    case GL_FLOAT_VEC2:
        return "vec2";
    case GL_FLOAT_VEC3:
        return "vec3";
    case GL_FLOAT_VEC4:
        return "vec4";
    case GL_DOUBLE:
        return "double";
    case GL_INT:
        return "int";
    case GL_UNSIGNED_INT:
        return "unsigned int";
    case GL_BOOL:
        return "bool";
    case GL_FLOAT_MAT2:
        return "mat2";
    case GL_FLOAT_MAT3:
        return "mat3";
    case GL_FLOAT_MAT4:
        return "mat4";
    default:
        return "?";
    }
}