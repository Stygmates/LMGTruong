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

bool My3DModel::loadModel(const char* filename)
{
	bool statusOK = true;
	Loader *loader = new Loader();
	loader->import( filename );
	loader->loadData( this->positions );
	this->initializeVertexBuffers();
	this->initializeVertexArray();
	return statusOK;
}

bool My3DModel::initializeVertexBuffers()
{
	bool statusOK = true;
	std::vector< std::vector< glm::vec3 > >::iterator it;
	for( it = this->positions.begin(); it != this->positions.end(); it++ )
	{
		//Vertex buffer object containing values
		GLuint vboID;
		//Generate its id
		glGenBuffers( 1, &vboID );
		//Bind vbo as current vbo
		glBindBuffer( GL_ARRAY_BUFFER, vboID );
		//Send data from CPU to GPU
		glBufferData(GL_ARRAY_BUFFER, it->size() * sizeof( glm::vec3 ), it->data(), GL_STATIC_DRAW );
		//Unbind buffer
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		//Add vboID to the vbo
		this->vertexBuffer.push_back( vboID );
	}
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
	int i = 0;
	for( it = this->vertexBuffer.begin(); it != this->vertexBuffer.end(); ++it )
	{
		int index = it - this->vertexBuffer.begin();
		// Bind vbo as the current buffer
		glBindBuffer( GL_ARRAY_BUFFER, this->vertexBuffer.at( index ) );
		// - specify the location and data format of the array of generic vertex attributes at index​ to use when rendering
		glVertexAttribPointer( i/*index of the generic vertex attribute: VBO index (not its ID!)*/, 3/*nb elements in the attribute: (x,y,z)*/, GL_FLOAT/*type of data*/, GL_FALSE/*normalize data*/, 0/*stride*/, 0/*offset in memory*/ );
		// - enable or disable a generic vertex attribute array
		glEnableVertexAttribArray( i/*index of the generic vertex attribute*/ );
		// Unbind vbo
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		i++;
	}
	//Unbind vao
	glBindVertexArray( 0 );

	return statusOK;
}

bool My3DModel::displayVertexBuffers()
{
	    std::vector< std::vector< glm::vec3 > >::iterator it;
		for( it = this->positions.begin(); it != this->positions.end(); ++it )
		{
			std::vector< glm::vec3 >::iterator it2;
			for( it2 = it->begin(); it2 != it->end(); ++it2 )
			{
				std::cout << it2->x << ", " << it2->y << ", " << it2->z << std::endl;
			}
		}
}
