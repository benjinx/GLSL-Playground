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
    shader->Load("basic.vert.glsl", "basic.frag.glsl");
    shader->SendTriangleData();
    //shader->PrintExtensions();
    //shader->LoadShaderProgramAsBinary();
    //shader->LoadShaderProgramAsSPRIV();



    // Window loop
    while(!glfwWindowShouldClose(window->GetWindow()))
    {
        // Input
        processInput(window->GetWindow());

        // Render here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader->Render();

        // Check and call events/swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window->GetWindow());
    }

    return 0;
}