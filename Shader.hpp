#ifndef __SHADER_HPP
#define __SHADER_HPP
#include <string>
#include <GL/glew.h>
#include <GL/gl.h>
// - GLUT
#include <GL/glut.h>
class Shader
{
public:
    Shader();
    Shader(std::string vertexFilename, std::string fragmentFilename);
    bool init();
    GLuint programID;

  private:
    std::string vertexFilename;
    std::string fragmentFilename;
    std::string vertexShaderContent;
    std::string fragmentShaderContent;
    bool getFileContent(const std::string &pFilename, std::string &pFileContent);
    bool checkShader(GLuint shader);
};
#endif