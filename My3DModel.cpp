#include "Loader.hpp"
#include <GL/glew.h>
//Use std::cout and std::endl
#include <iostream>

#include "My3DModel.hpp"


My3DModel::My3DModel()
{
}

My3DModel::~My3DModel()
{
}

bool My3DModel::loadModel(std::string filename)
{
	bool statusOK = true;
	Loader *loader = new Loader();
	loader->import( filename );
	loader->loadData( this->positions , this->indexes, this->texCoords );
	this->initializeVertexBuffer();
	this->initializeVertexArray();
	this->initializeIndexes();
	return statusOK;
}

bool My3DModel::loadTexture(std::string textureFilename)
{
	bool statusOK = true;
	this->texture = new Texture( textureFilename );
	return statusOK;
}

// bool My3DModel::initializeVertexBuffers()
// {
// 	bool statusOK = true;
// 	std::vector< std::vector< glm::vec3 > >::iterator it;
// 	for( it = this->positions.begin(); it != this->positions.end(); it++ )
// 	{
// 		//Vertex buffer object containing values
// 		GLuint vboID;
// 		//Generate its id
// 		glGenBuffers( 1, &vboID );
// 		//Bind vbo as current vbo
// 		glBindBuffer( GL_ARRAY_BUFFER, vboID );
// 		//Send data from CPU to GPU
// 		glBufferData(GL_ARRAY_BUFFER, it->size() * sizeof( glm::vec3 ), it->data(), GL_STATIC_DRAW );
// 		//Unbind buffer
// 		glBindBuffer( GL_ARRAY_BUFFER, 0 );
// 		//Add vboID to the vbo
// 		this->vertexBuffer.push_back( vboID );
// 	}
// 	return statusOK;
// }

bool My3DModel::initializeVertexBuffer()
{
	bool statusOK = true;
	//Generate its id
	glGenBuffers(1, &vertexBuffer);
	//Bind vbo as current vbo
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	//Send data from CPU to GPU
	glBufferData(GL_ARRAY_BUFFER, this->positions.size() * sizeof(glm::vec3), this->positions.data(), GL_STATIC_DRAW);
	//Unbind buffer
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	return statusOK;
}

bool My3DModel::initializeIndexes()
{
	bool statusOK = true;

	// Index buffer
	// - this buffer is used to separate topology from positions: send points + send toplogy (triangle: 3 vertex indices)
    glGenBuffers( 1, &this->indexBuffer );
    // buffer courant a manipuler
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
    // definit la taille du buffer et le remplit
	int	numberOfIndices_ = static_cast< int >( this->indexes.size() );
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, numberOfIndices_ * sizeof( GLuint ), this->indexes.data(), GL_STATIC_DRAW );
    // buffer courant : rien
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	return statusOK;
}


bool My3DModel::initializeVertexArray()
{
	bool statusOK = true;
	// Generate vao name
	glGenVertexArrays( 1, &vertexArray );
	// Bind vao as the current vao
	glBindVertexArray(this->vertexArray);
	std::vector< GLuint >::iterator it;
	// Bind vbo as the current buffer
	glBindBuffer( GL_ARRAY_BUFFER, this->vertexBuffer );
	// - specify the location and data format of the array of generic vertex attributes at index​ to use when rendering
	glVertexAttribPointer( 0/*index of the generic vertex attribute: VBO index (not its ID!)*/, 3/*nb elements in the attribute: (x,y,z)*/, GL_FLOAT/*type of data*/, GL_FALSE/*normalize data*/, 0/*stride*/, 0/*offset in memory*/ );
	// - enable or disable a generic vertex attribute array
	glEnableVertexAttribArray( 0/*index of the generic vertex attribute*/ );
	// Unbind vbo
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	//Unbind vao
	glBindVertexArray( 0 );

	return statusOK;
}

// bool My3DModel::displayVertexBuffers()
// {
// 	bool statusOK = true;
// 	std::vector< std::vector< glm::vec3 > >::iterator it;
// 	for( it = this->positions.begin(); it != this->positions.end(); ++it )
// 	{
// 		std::vector< glm::vec3 >::iterator it2;
// 		for( it2 = it->begin(); it2 != it->end(); ++it2 )
// 		{
// 			std::cout << it2->x << ", " << it2->y << ", " << it2->z << std::endl;
// 		}
// 	}
// 	return statusOK;
// }

// int My3DModel::getNbVertices()
// {
// 	int i = 0;
// 	std::vector< std::vector < glm::vec3 > >::iterator it;
// 	it = this->positions.begin();
// 	for(it = this->positions.begin(); it != this->positions.end(); it++)
// 	{
// 		std::cerr << "Partie avec " << it->size() << " sommets." << std::endl; 
// 		i += it->size();
// 	}
// 	std::cerr << "Nombre de sommets total: " << i << std::endl;
// 	return i;
// }

int My3DModel::getNbVertices()
{
	return this->positions.size();
}