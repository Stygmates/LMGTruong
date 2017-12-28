#ifndef __MY3DMODEL_HPP
#define __MY3DMODEL_HPP
#include <GL/gl.h>
#ifndef LOADER
#include "MeshLoader.hpp"
#endif
#ifdef LOADER
#include "Loader.hpp"
#endif
#include "Texture.hpp"

class My3DModel{
private:
    Loader *loader;
    void normalize(std::vector<glm::vec3> &positions);

  public:
    My3DModel();
    ~My3DModel();

    Texture *texture;
    
    bool loadModel(std::string filename);
    bool loadTexture(std::string textureFilename);
    //bool initializeVertexBuffers();
    bool initializeVertexBuffer();
    bool initializeIndexesBuffer();
    bool initializeTexturesBuffer();
    bool initializeVertexArray();
    bool displayVertexBuffers();
    int getNbVertices();

    GLuint vertexID;
    GLuint indexID;
    GLuint textureID;
    GLuint vaoID;

    std::vector<glm::vec3> positions;
    std::vector<unsigned int> indexes;
    std::vector<glm::vec2> texCoords;
};
#endif