#ifndef CHAPTER_3_HPP
#define CHAPTER_3_HPP

#include <OpenGL.hpp>

#include <GLSLProgram.hpp>
#include <Window.hpp>

class Chapter3 {
public:
    void Start();
    void ProcessInput(GLFWwindow* window);

private:
    GLSLProgram* _mProgram;
    Window* _mWindow;
};

#endif // CHAPTER_3_HPP