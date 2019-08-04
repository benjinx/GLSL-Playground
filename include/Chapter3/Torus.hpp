#ifndef TORUS_HPP
#define TORUS_HPP

#include <OpenGL.hpp>
#include <Chapter3/TriangleMesh.hpp>

class Torus : public TriangleMesh
{
public:
    Torus(GLfloat outerRadius, GLfloat innerRadius, GLuint nsides, GLuint nrings);
};

#endif // TORUS_HPP