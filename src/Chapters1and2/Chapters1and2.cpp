#include <Chapters1and2/Chapters1and2.hpp>

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include <OpenGL.hpp>
#include <Chapters1and2/Shader.hpp>
#include <Utils.hpp>
#include <Window.hpp>


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void APIENTRY debugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* param)
{
    // Convert GLenum parameters to strings 
    printf("%s:%s[%s](%d): %s\n", glGetString(source), glGetString(type), glGetString(severity), id, message);
}


void Chapters1and2::Start()
{
    // Set asset path
    Utils::SetAssetPath(RESOURCE_PATH);

    int width = 800,
        height = 600;

    Window* window = new Window(width, height);

    Shader* shader = new Shader();

    glDebugMessageCallback(debugCallback, nullptr);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, GL_TRUE);

    shader->PrintVersions();
    //shader->PrintExtensions();
    //shader->LoadShaderProgramAsBinary(); // must load shader first to save as binary (shader->Load("...", "...");)
    //shader->LoadShaderProgramAsSPRIV();

    // Triangle Example
    //shader->Load("basic.vert.glsl", "basic.frag.glsl");
    //shader->GetActiveVertexInputAttribs();
    //shader->GetActiveUniformVariables();
    //shader->SendTriangleData();

    // Blob Example
    //shader->Load("blob.vert.glsl", "blob.frag.glsl");
    //shader->SendBlobData();

    // Pipeline Example
    //shader->CreateShaderProgramViaPipeline1();
    shader->CreateShaderProgramViaPipeline2();
    shader->CreatePipeLines();
    shader->CreateVAO();

    // Window loop
    while (!glfwWindowShouldClose(window->GetWindow()))
    {
        // Input
        processInput(window->GetWindow());

        // Render
        //shader->Render();
        shader->RenderPipelines(window->GetWidth(), window->GetHeight());

        // Check and call events/swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window->GetWindow());
    }
}