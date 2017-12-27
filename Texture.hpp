#ifndef __TEXTURE_HPP
#define __TEXTURE_HPP
#include <string>
#include <GL/gl.h>

class Texture
{
private:
    GLuint texture;
public:
    Texture( std::string filename );
};
#endif