#include <Chapters1and2/Window.hpp>

////////// CALL BACKS //////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
//////////////////////////////

Window::Window(int width, int height)
{
    // Start with a window using GLFW
    // Inits for glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    _mWidth = width;
    _mHeight = height;

    // Create our window
    _mWindow = glfwCreateWindow(width, height, "GLSL Playground", nullptr, nullptr);

    if (_mWindow == nullptr)
    {
        printf("Failed to create GLFW Window: \n");
        glfwTerminate();
    }

    // Declare our current context
    glfwMakeContextCurrent(_mWindow);

    // v-sync
    glfwSwapInterval(0);

    // Attach via glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Create framebuffersize and set the viewport
    glfwGetFramebufferSize(_mWindow, &width, &height);
    glViewport(0, 0, width, height);

    // Setting resize for the window via callback.
    glfwSetFramebufferSizeCallback(_mWindow, framebuffer_size_callback);
}

Window::~Window()
{
    // Clean up before finishing
    glfwTerminate();
}

