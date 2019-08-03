#include <Chapter3/Chapter3.hpp>

#include <Utils.hpp>
#include <Window.hpp>

void Chapter3::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void Chapter3::Start()
{
    Utils::SetAssetPath(RESOURCE_PATH);
    
    int width = 800,
        height = 600;

    Window* window = new Window(width, height);

    while (!glfwWindowShouldClose(window->GetWindow()))
    {
        // Input
        ProcessInput(window->GetWindow());

        // Render

        // Check and call events/swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window->GetWindow());
    }
}