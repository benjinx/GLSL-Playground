#include <Chapter3/Chapter3.hpp>

#include <GLSLProgram.hpp>
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

    GLSLProgram* prog = new GLSLProgram();
    
    try {

        std::string vert = "/Chapter3/basic.vert.glsl";
        prog->CompileShader(vert.c_str());

        std::string frag = "/Chapter3/basic.frag.glsl";
        prog->CompileShader(frag.c_str());

        prog->Link();
        prog->Validate();
        prog->Use();
    }
    catch (GLSLProgramException & e)
    {
        std::cerr << e.what() << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    // Test pass in stuff, doesn't actually do anything in this demo.
    glm::mat4 matrix = glm::mat4(1);
    prog->SetUniform("ModelViewMatrix", matrix);
    prog->SetUniform("LightPosition", 1.0f, 1.0f, 1.0f);

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