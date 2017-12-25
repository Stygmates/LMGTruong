#ifndef __MY3DMODEL_HPP
#define __MY3DMODEL_HPP
#include <GL/gl.h>
#include "Loader.hpp"

class My3DModel{
private:
    Loader *loader;
    std::vector< GLuint > vertexBuffer;
    std::vector< std::vector< glm::vec3 > > positions;
public:
    My3DModel();
    ~My3DModel();
    bool loadModel(std::string filename);
    bool initializeVertexBuffers();
    bool initializeIndexes();
    bool initializeVertexArray();
    GLuint vertexArray;
    bool displayVertexBuffers();
    int getNbVertices();
    std::vector< unsigned int > indexes;
    GLuint indexBuffer;
};
#endif