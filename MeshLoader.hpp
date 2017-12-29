#ifndef MESHLOADER_H
#define MESHLOADER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <stdio.h>
#include <string.h>
#include <iostream>
class Loader
{
private:
	const char* filename;
public:
	Loader();
	~Loader();
	void import(std::string filename);
	void loadData(std::vector<glm::vec3> &positions, std::vector<unsigned int> &faces, std::vector<glm::vec2> &texCoords);
	void textureCoordsFill(std::vector<glm::vec2> &texCoords2, const std::vector<glm::vec2> texCoords, const std::vector<unsigned int> texIndexes);
};
#endif