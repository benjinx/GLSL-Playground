#include <Chapter4/Chapter4.hpp>

#include <Utils.hpp>

void Chapter4::ProcessInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}

void Chapter4::Start()
{
    Utils::SetAssetPath(RESOURCE_PATH);

    int width = 800,
        height = 600;

    _mWindow = new Window(width, height);

    _mProgram = new GLSLProgram();

    try {

        std::string vert = "/Chapter4/basic.vert.glsl";
        _mProgram->CompileShader(vert.c_str());

        std::string frag = "/Chapter4/basic.frag.glsl";
        _mProgram->CompileShader(frag.c_str());

        _mProgram->Link();
        _mProgram->Validate();
        _mProgram->Use();
    }
    catch (GLSLProgramException & e)
    {
        std::cerr << e.what() << std::endl;
        system("pause");
        exit(EXIT_FAILURE);
    }

    // Test pass in stuff, doesn't actually do anything in this demo.
    glm::mat4 matrix = glm::mat4(1);
    _mProgram->SetUniform("ModelViewMatrix", matrix);
    _mProgram->SetUniform("LightPosition", 1.0f, 1.0f, 1.0f);

    while (!glfwWindowShouldClose(_mWindow->GetWindow()))
    {
        // Input
        ProcessInput(_mWindow->GetWindow());

        // Render

        // Check and call events/swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(_mWindow->GetWindow());
    }
}