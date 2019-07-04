#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

////////// CALL BACKS //////////
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
//////////////////////////////

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }
}


int main()
{
    int width = 800,
        height = 600;

    // Start with a window using GLFW
    // Inits for glfw
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create our window
    GLFWwindow* window = glfwCreateWindow(width, height, "GLSL Playground", nullptr, nullptr);

    if (window == nullptr)
    {
        printf("Failed to create GLFW Window: \n");
        glfwTerminate();
    }

    // Declare our current context
    glfwMakeContextCurrent(window);

    // v-sync
    glfwSwapInterval(0);

    // Attach via glad
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    // Create framebuffersize and set the viewport
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    // Setting resize for the window via callback.
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    // Extensions
    //GLint nExtensions;
    //glGetIntegerv(GL_NUM_EXTENSIONS, &nExtensions);

    //for (int i = 0; i < nExtensions; i++)
    //    printf("%s\n", glGetStringi(GL_EXTENSIONS, i));

    // It says resource_path isn't valid but it is.
    printf("%s\n", RESOURCE_PATH);
    
    // Window loop
    while(!glfwWindowShouldClose(window))
    {
        // Input
        processInput(window);

        // Render here
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Check and call events/swap the buffers
        glfwPollEvents();
        glfwSwapBuffers(window);
    }

    // Clean up before finishing
    glfwTerminate();

    return 0;
}