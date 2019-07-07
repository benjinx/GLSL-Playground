#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>

#include <OpenGL.hpp>

class Window
{
public:
    Window(int width, int height);
    ~Window();

    GLFWwindow* GetWindow() { return _mWindow; }

private:
    GLFWwindow* _mWindow;
};

#endif // WINDOW_HPP