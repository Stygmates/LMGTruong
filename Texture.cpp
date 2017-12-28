#include "Texture.hpp"
#include <SOIL.h>
#include <cassert>
#include <GL/gl.h>

Texture::Texture(std::string filename)
{
    if( !filename.empty() )
    {
        glGenTextures(1, &texture);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        
        int textureWidth, textureHeight;
        unsigned char *image = SOIL_load_image(filename.c_str(), &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
        assert( image != NULL );
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        SOIL_free_image_data(image);
        this->textureLoaded = true;
    }
    else
    {
        this->textureLoaded = false;
    }
}