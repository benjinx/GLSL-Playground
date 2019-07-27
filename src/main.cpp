#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include <OpenGL.hpp>
#include <Shader.hpp>
#include <Utils.hpp>
#include <Window.hpp>


void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


int main()
{
    // Set asset path
    Utils::SetAssetPath(RESOURCE_PATH);

    int width = 800,
        height = 600;

    Window* window = new Window(width, height);

    Shader* shader = new Shader();

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
    shader->CreateShaderProgramViaPipeline1();

    // Window loop
    while(!glfwWindowShouldClose(window->GetWindow()))
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

    return 0;
}