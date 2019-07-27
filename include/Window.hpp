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

    int GetWidth() { return _mWidth; }
    int GetHeight() { return _mHeight; }

private:
    GLFWwindow* _mWindow;
    int _mWidth, _mHeight;
};

#endif // WINDOW_HPP