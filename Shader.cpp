#include "Shader.hpp"
#include <fstream>
#include <string>
#include <iostream>
#include <GL/glew.h>
#include <GL/gl.h>
// - GLUT
#include <GL/glut.h>

Shader::Shader(std::string vertexFilename, std::string fragmentFilename)
{
    this->vertexFilename = vertexFilename;
    this->fragmentFilename = fragmentFilename;
}

bool Shader::init()
{
    std::cout << "Chargement du shader" << std::endl;
    bool statusOK = true;
    statusOK = this->getFileContent( this->vertexFilename, this->vertexShaderContent );
    if(!statusOK)
    {
        return statusOK;
    }
    statusOK = this->getFileContent( this->fragmentFilename, this->fragmentShaderContent );
    if(!statusOK)
        return statusOK;

    const char *vertexShaderSource = this->vertexShaderContent.c_str();

    const char *fragmentShaderSource = this->fragmentShaderContent.c_str();

    GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);

    GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    //

    glShaderSource(vertexShaderID, 1, &vertexShaderSource, nullptr);
    glShaderSource(fragmentShaderID, 1, &fragmentShaderSource, nullptr);

    //Compile the shaders
    glCompileShader(vertexShaderID);
    if (!checkShader(vertexShaderID))
        return false;

    glCompileShader(fragmentShaderID);
    if (!checkShader(fragmentShaderID))
        return false;

    //Create the program
    programID = glCreateProgram();
    //Attach the shades to the program
    glAttachShader(this->programID, vertexShaderID);
    glAttachShader(this->programID, fragmentShaderID);
    //Link the program
    glLinkProgram(this->programID);
    std::cout << "Shader chargé avec succès" << std::endl;
    return statusOK;
}

/******************************************************************************
 * Helper function used to load shader source code from files
 *
 * @param pFilename ...
 *
 * @return ...
 ******************************************************************************/
bool Shader::getFileContent(const std::string &pFilename, std::string &pFileContent)
{
    std::ifstream file(pFilename.c_str(), std::ios::in);
    if (file)
    {
        // Initialize a string to store file content
        file.seekg(0, std::ios::end);
        pFileContent.resize(file.tellg());
        file.seekg(0, std::ios::beg);

        // Read file content
        file.read(&pFileContent[0], pFileContent.size());

        // Close file
        file.close();

        return true;
    }
    else
    {
        std::cerr << "Ouverture du fichier " << pFilename << " impossible." << std::endl;
        return false;
    }
}

/******************************************************************************
 * Errors check
 ******************************************************************************/

bool Shader::checkShader(GLuint shader)
{
    // Check compilation status
    GLint compileStatus;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);
    if (compileStatus == GL_FALSE)
    {
        GLint logInfoLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logInfoLength);
        if (logInfoLength > 0)
        {
            // Return information log for shader object
            GLchar *infoLog = new GLchar[logInfoLength];
            GLsizei length = 0;
            glGetShaderInfoLog(shader, logInfoLength, &length, infoLog);
            // LOG
            std::cout << infoLog << std::endl;
        }
        return false;
    }
    return true;
}