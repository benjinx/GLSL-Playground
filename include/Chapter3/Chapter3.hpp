#ifndef CHAPTER_3_HPP
#define CHAPTER_3_HPP

#include <OpenGL.hpp>

#include <GLSLProgram.hpp>
#include <Window.hpp>

class Chapter3 {
public:
    void Start();
    void ProcessInput(GLFWwindow* window);

    void CompileAndLink();

private:
    GLSLProgram* _mProgram;
    Window* _mWindow;

    glm::mat4 _mModel, _mView, _mProjection;
};

#endif // CHAPTER_3_HPP