#include <Chapters1and2/Chapters1and2.hpp>

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include <Chapters1and2/ShaderC1.hpp>
#include <Utils.hpp>
#include <Window.hpp>

#include <GLSLProgram.hpp>

#include <iostream>



void Chapters1and2::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

std::string SourceToString(GLenum source)
{
    switch (source)
    {
    case GL_DEBUG_SOURCE_API:               return "GL_DEBUG_SOURCE_API";
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     return "GL_DEBUG_SOURCE_WINDOW_SYSTEM";
    case GL_DEBUG_SOURCE_SHADER_COMPILER:   return "GL_DEBUG_SOURCE_SHADER_COMPILER";
    case GL_DEBUG_SOURCE_THIRD_PARTY:       return "GL_DEBUG_SOURCE_THIRD_PARTY";
    case GL_DEBUG_SOURCE_APPLICATION:       return "GL_DEBUG_SOURCE_APPLICATION";
    case GL_DEBUG_SOURCE_OTHER:             return "GL_DEBUG_SOURCE_OTHER";
    default:                                return NULL;
    }
}

std::string TypeToString(GLenum type)
{
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:               return "GL_DEBUG_TYPE_ERROR";
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: return "GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR";
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  return "GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR";
    case GL_DEBUG_TYPE_PORTABILITY:         return "GL_DEBUG_TYPE_PORTABILITY";
    case GL_DEBUG_TYPE_PERFORMANCE:         return "GL_DEBUG_TYPE_PERFORMANCE";
    case GL_DEBUG_TYPE_MARKER:              return "GL_DEBUG_TYPE_MARKER";
    case GL_DEBUG_TYPE_PUSH_GROUP:          return "GL_DEBUG_TYPE_PUSH_GROUP";
    case GL_DEBUG_TYPE_POP_GROUP:           return "GL_DEBUG_TYPE_POP_GROUP";
    case GL_DEBUG_TYPE_OTHER:               return "GL_DEBUG_TYPE_OTHER";
    default:                                return NULL;
    }
}

std::string SeverityToString(GLenum severity)
{
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_HIGH:            return "GL_DEBUG_SEVERITY_HIGH";
    case GL_DEBUG_SEVERITY_MEDIUM:          return "GL_DEBUG_SEVERITY_MEDIUM";
    case GL_DEBUG_SEVERITY_LOW:             return "GL_DEBUG_SEVERITY_LOW";
    case GL_DEBUG_SEVERITY_NOTIFICATION:    return "GL_DEBUG_SEVERITY_NOTIFICATION";
    default:                                return NULL;
    }
}

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* param)
{
    // Convert GLenum parameters to strings
    std::string SourceString = SourceToString(source);
    std::string TypeString = TypeToString(type);
    std::string SeverityString = SeverityToString(severity);

    printf("%s:%s[%s](%d): %s\n", SourceString.c_str(), TypeString.c_str(), SeverityString.c_str(), id, message);
}


void Chapters1and2::Start()
{
    // Set asset path
    Utils::SetAssetPath(RESOURCE_PATH);

    int width = 800,
        height = 600;

    Window* window = new Window(width, height);

    //Shader* shader = new Shader();

    glDebugMessageCallback(debugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

    //shader->PrintVersions();
    //shader->PrintExtensions();
    //shader->LoadShaderProgramAsBinary(); // must load shader first to save as binary (shader->Load("...", "...");)
    //shader->LoadShaderProgramAsSPRIV();

    // Triangle Example
    //shader->Load("Chapters1and2/basic.vert.glsl", "Chapters1and2/basic.frag.glsl");
    //shader->GetActiveVertexInputAttribs();
    //shader->GetActiveUniformVariables();
    //shader->SendTriangleData();

    // Blob Example
    //shader->Load("Chapters1and2/blob.vert.glsl", "Chapters1and2/blob.frag.glsl");
    //shader->SendBlobData();

    // Pipeline Example
    //shader->CreateShaderProgramViaPipeline1();
    //shader->CreateShaderProgramViaPipeline2();
    //shader->CreatePipeLines();
    //shader->CreateVAO();

    // Shader Program Example
    GLSLProgram prog;

    try {
        std::string vert = "Chapters1and2/basic.vert.glsl";
        //std::cout << "vert: " << vert << "\n";
        prog.CompileShader(vert.c_str());

        std::string frag = "Chapters1and2/basic.frag.glsl";
        //std::cout << "frag: " << frag << "\n";
        prog.CompileShader(frag.c_str());
        
        prog.Link();
        prog.Validate();
        prog.Use();
    }
    catch (GLSLProgramException & e)
    {
        std::cerr << e.what() << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    // Test pass in stuff, doesn't actually do anything in this demo.
    glm::mat4 matrix = glm::mat4(1);
    prog.SetUniform("ModelViewMatrix", matrix);
    prog.SetUniform("LightPosition", 1.0f, 1.0f, 1.0f);

    // Window loop
    while (!glfwWindowShouldClose(window->GetWindow()))
    {
        // Input
        ProcessInput(window->GetWindow());

        // Render
        //shader->Render();
        //shader->RenderPipelines(window->GetWidth(), window->GetHeight());

        // Check and call events/swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window->GetWindow());
    }
}