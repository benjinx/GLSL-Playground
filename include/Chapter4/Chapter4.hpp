#ifndef CHAPTER_4_HPP
#define CHAPTER_4_HPP

#include <OpenGL.hpp>

#include <GLSLProgram.hpp>
#include <Window.hpp>

class Chapter4 {
public:
    void Start();
    void ProcessInput(GLFWwindow* window);

private:
    GLSLProgram * _mProgram;
    Window* _mWindow;
};

#endif // CHAPTER_4_HPP