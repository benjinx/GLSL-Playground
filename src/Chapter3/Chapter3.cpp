#include <Chapter3/Chapter3.hpp>

#include <Utils.hpp>


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

    _mWindow = new Window(width, height);

    _mProgram = new GLSLProgram();

    CompileAndLink();

    glEnable(GL_DEPTH_TEST);

    _mModel = glm::mat4(1.0f);
    _mModel = glm::rotate(_mModel, glm::radians(-35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    _mModel = glm::rotate(_mModel, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    _mView = glm::lookAt(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    _mProjection = glm::mat4(1.0f);

    _mProgram->SetUniform("Kd", 0.9f, 0.5f, 0.3f);
    _mProgram->SetUniform("Ld", 1.0f, 1.0f, 1.0f);
    _mProgram->SetUniform("LightPosition", _mView * glm::vec4(5.0f, 5.0f, 2.0f, 1.0f));

    while (!glfwWindowShouldClose(_mWindow->GetWindow()))
    {
        // Input
        ProcessInput(_mWindow->GetWindow());

        // Render
        // Clear to render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render the triangle
        glBindVertexArray(_mProgram->GetHandle());
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Check and call events/swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(_mWindow->GetWindow());
    }
}

void Chapter3::CompileAndLink()
{
    try {

        std::string vert = "/Chapter3/basic.vert.glsl";
        _mProgram->CompileShader(vert.c_str());

        std::string frag = "/Chapter3/basic.frag.glsl";
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
}