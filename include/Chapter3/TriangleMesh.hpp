#ifndef TRIANGLE_MESH_HPP
#define TRIANGLE_MESH_HPP

#include <vector>

#include <OpenGL.hpp>
#include <Chapter3/Drawable.hpp>

class TriangleMesh : public Drawable {

protected:

    GLuint nVerts;     // Number of vertices
    GLuint vao;        // The Vertex Array Object

                       // Vertex buffers
    std::vector<GLuint> buffers;

    virtual void initBuffers(
        std::vector<GLuint> * indices,
        std::vector<GLfloat> * points,
        std::vector<GLfloat> * normals,
        std::vector<GLfloat> * texCoords = nullptr,
        std::vector<GLfloat> * tangents = nullptr
    );

    virtual void deleteBuffers();

public:
    virtual ~TriangleMesh();
    virtual void render() const;
    GLuint getVao() const { return vao; }
    GLuint getElementBuffer() { return buffers[0]; }
    GLuint getPositionBuffer() { return buffers[1]; }
    GLuint getNormalBuffer() { return buffers[2]; }
    GLuint getTcBuffer() { if (buffers.size() > 3) return buffers[3]; else return 0; }
    GLuint getNumVerts() { return nVerts; }
};

#endif // TRIANGLE_MESH_HPP