#include "Texture.hpp"
#include <SOIL.h>
#include <cassert>
#include <GL/gl.h>

Texture::Texture(std::string filename)
{
    int textureWidth, textureHeight;
    unsigned char *image = SOIL_load_image(filename.c_str(), &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
    assert( image != NULL );
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
}