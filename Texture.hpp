#ifndef __TEXTURE_HPP
#define __TEXTURE_HPP
#include <string>
#include <GL/gl.h>

class Texture
{
private:
    bool textureLoaded;
public:
  Texture(std::string filename);
  GLuint texture;
};
#endif