
/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// STL
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <vector>
#include <unistd.h>

// System
#include <cstdio>
#include <cassert>

// Graphics
// - GLEW (always before "gl.h")
#include <GL/glew.h>
// - GL
#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>
// - GLUT
#include <GL/glut.h>


#define GLM_ENABLE_EXPERIMENTAL

// glm
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Soil
#include <SOIL.h>

//Custom classes
#include "My3DModel.hpp"
#include "Shader.hpp"

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

// VBO (vertex buffer object) : used to store positions coordinates at each point
GLuint positionBuffer;
// VBO (vertex buffer object) : used to store normales at each point
GLuint normalBuffer;
// VBO (vertex buffer object) : used to store positions index
GLuint indexBuffer;
// VAO (vertex array object) : used to encapsulate several VBO
GLuint vertexArray;

// Mesh
int numberOfVertices_;
int numberOfIndices_;

// Shader program
GLuint shaderProgram;

// Camera parameters
// - view
glm::vec3 _cameraEye;
glm::vec3 _cameraCenter;
glm::vec3 _cameraUp;
float pan = 0.01f ;
// - projection
float _cameraFovY;
float _cameraAspect;
float _cameraZNear;
float _cameraZFar;

// Mesh parameters
glm::vec3 _meshColor;

/**
 * Cube map
 */
	GLuint texture;
// - mesh
GLuint mCubemapVertexArray;
GLuint mCubemapVertexBuffer;
GLuint mCubemapIndexBuffer;
// - shader
GLuint mCubeMapShaderProgram;
// - texture
// ...

// Data directory
std::string dataRepository;

My3DModel model;
Shader *shader;

std::string objname;
std::string texturename;
bool useMeshAnimation = true; // TODO: use keyboard to activate/deactivate
bool useRotationAnimation = true;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
***************************** METHOD DEFINITION ******************************
******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

bool initialize();
bool checkExtensions();
bool initializeArrayBuffer();
bool initializeVertexArray();
bool initializeShaderProgram();
void initializeCamera();
bool finalize();

// Cubemap functions
bool initializeCubemap();
bool initializeCubemapGeometry();
bool initializeCubemapTextures();
bool initializeCubemapShader();

/******************************************************************************
 * Initialize cubemap
 ******************************************************************************/
bool initializeCubemap()
{
	bool statusOK = true;

	std::cout << "Initialize cubemap..." << std::endl;

	if ( statusOK )
	{
		statusOK = initializeCubemapGeometry();
	}

	if ( statusOK )
	{
		statusOK = initializeCubemapTextures();
	}

	if ( statusOK )
	{
		statusOK = initializeCubemapShader();
	}

	return statusOK;
}

/******************************************************************************
 * Initialize cubemap geometry
 ******************************************************************************/
bool initializeCubemapGeometry()
{
	bool result = true;

	std::cout << "- initialize geometry..." << std::endl;

	// Allocate GL resources
	glGenBuffers( 1, &mCubemapVertexBuffer );
	glGenBuffers( 1, &mCubemapIndexBuffer );
	glGenVertexArrays( 1, &mCubemapVertexArray );

	//----------------------------------------
	// Position buffer initialization
	// - geometry
	//----------------------------------------
	std::vector< GLfloat > vertexData =
	{
		// front vetices: z=1
		-1.f, -1.f, 1.f,
		1.f, -1.f, 1.f,
		1.f, 1.f, 1.f,
		-1.f, 1.f, 1.f,
		// rear vertices: z=-1
		-1.f, -1.f, -1.f,
		1.f, -1.f, -1.f,
		1.f, 1.f, -1.f,
		-1.f, 1.f, -1.f
	};
	glBindBuffer( GL_ARRAY_BUFFER, mCubemapVertexBuffer);
	glBufferData( GL_ARRAY_BUFFER, vertexData.size() * sizeof( GLfloat ), static_cast< const void* >( vertexData.data() ), GL_STATIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	//----------------------------------------
	// Index buffer initialization
	// - topology : faces
	//----------------------------------------
	std::vector< GLuint > indexData =
	{
		7, 4, 5,
		5, 6, 7,

		0, 4, 7,
		7, 3, 0,

		5, 1, 2,
		2, 6, 5,

		0, 3, 2,
		2, 1, 0,


		7, 6, 2,
		2, 3, 7,

		4, 0, 5,
		5, 0, 1
	};
	/*
	{
		// Bottom face
		1, 0, 5,
		5, 4, 0,
		// Top face
		3, 2, 7,
		7, 6, 2,
		// Left face
		4, 0, 7,
		7, 3, 0,
		// Right face
		1, 5, 2,
		2, 6, 5,
		// Front face
		0, 1, 3,
		3, 2, 1,
		// Rear face
		5, 4, 6,
		6, 7, 4
	};
	*/
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mCubemapIndexBuffer );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, indexData.size() * sizeof( GLuint ) , static_cast< const void* >( indexData.data() ), GL_STATIC_DRAW );

	// Reset GL state(s)
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
	
	//----------------------------------------
	// Vertex array configuration
	// - main container of all vertex attributes
	//----------------------------------------
	glBindVertexArray( mCubemapVertexArray );
	// - position
	glEnableVertexAttribArray( 0/*attribute index*/ ); // same as in the shader program (see: "layout (location = 0)")
	glBindBuffer( GL_ARRAY_BUFFER, mCubemapVertexBuffer );
	glVertexAttribPointer( 0/*attribute index*/, 3/*nb components per vertex*/, GL_FLOAT/*type*/, GL_FALSE/*un-normalized*/, 0/*memory stride*/, static_cast< GLubyte* >( nullptr )/*byte offset from buffer*/ );
	// - required for indexed rendering (faces)
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, mCubemapIndexBuffer );
	
	// Reset GL state(s)
	glBindVertexArray( 0 );
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	return result;
}

/******************************************************************************
 * Initialize cubemap textures
 ******************************************************************************/
bool initializeCubemapTextures()
{
	bool result = true;




	//--------------------------------
	//--------------------------------
	//--------------------------------
	//--------------------------------
	//--------------------------------
	//--------------------------------

	std::cout << "- initialize textures..." << std::endl;
	
	// Initialize cubemap
	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	// Bind cubemap
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);

	// Cubemap parameters (filtering, wrapping, etc...)
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// Set the 6 faces cubemap filenames !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	// - be consistent with cubemap's internal OpenGL face ordering
	const std::string envmapRepository = dataRepository + "/cubemap/";
	std::vector< std::string > envmapTextures( 6 );
	envmapTextures[ 0 ] = "left.jpg";
	envmapTextures[ 1 ] = "right.jpg";
	envmapTextures[ 2 ] = "top.jpg";
	envmapTextures[ 3 ] = "bottom.jpg";
	envmapTextures[ 4 ] = "back.jpg";
	envmapTextures[ 5 ] = "front.jpg";

	// Fille the cubemap texture
	// - load 6 faces individually with your image library (ex: SOIL) in textures
	// - then send data to GPU
	const GLint level = 0;
	const GLint internalFormat = GL_RGB;
	const GLint border = 0;
	const GLenum format = GL_RGB;
	const GLenum type = GL_UNSIGNED_BYTE;
	for ( size_t i = 0; i < 6; ++i )
	{
		// Load texture from file (and convert to RGB)
		int textureWidth;
		int textureHeight;
		const std::string& textureFilename = envmapRepository + envmapTextures[ i ];
		unsigned char* image = SOIL_load_image( textureFilename.c_str(), &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB );

		assert( image != nullptr );

		// Upload data to device (GPU)
		const GLenum target = GL_TEXTURE_CUBE_MAP_POSITIVE_X + i;
		const GLsizei width = textureWidth;
		const GLsizei height = textureHeight;
		const GLvoid* pixels = static_cast< const GLvoid* >( image );
		glTexImage2D( target, level, internalFormat, width, height, border, format, type, pixels );


		// Free CPU memory
		SOIL_free_image_data( image );
	}


	// Reste GL state(s)
	// ...

	return result;
}

/******************************************************************************
* Initialize cubemap shader
******************************************************************************/
bool initializeCubemapShader()
{
	bool statusOK = true;

	std::cout << "- initialize shader program..." << std::endl;

	mCubeMapShaderProgram = glCreateProgram();

	GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
	GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

	// Vertex shader
	const char* vertexShaderSource[] = {
	    "#version 130                                                       \n"
		"precision mediump float;                                              \n"
		"// INPUT                                                              \n"
		"in vec3 position;			                               \n"
		"                                                                      \n"
		"// UNIFORM                                                            \n"
		"uniform mat4 uModelViewProjectionMatrix;                              \n"
		"                                                                      \n"
		"// OUTPUT                                                             \n"
		"out vec3 pos;"
	    "                                                                      \n"
		"// MAIN                                                               \n"
	    "void main( void )                                                     \n"
	    "{                                                                     \n"
		"    // Send position to Clip-space                                    \n"
	    "    gl_Position = uModelViewProjectionMatrix * vec4( position, 1.0 ); \n"
		"	 pos = position;													\n"
		"}                                                                     \n"
	};

	// Fragment shader
	const char *fragmentShaderSource[] = {
		"#version 130                                                    \n"
		"precision mediump float;                                           \n"
		"// INPUT                                                           \n"
		"in vec3 pos;														\n"
		"                                                                   \n"
		"// UNIFORM                                                         \n"
		"uniform samplerCube skybox;                                        \n"
		"// OUTPUT                                                          \n"
		"out vec4 fragmentColor;			                    \n"
		"                                                                   \n"
		"// MAIN                                                            \n"
		"void main( void )                                                  \n"
		"{                                                                  \n"
		"    vec4 color = vec4( 1.0, 0.0, 0.0, 1.0 );                       \n"
		"    fragmentColor = vec4( color.r, color.g, color.b, 1.0 );        \n"
		"	 fragmentColor = texture(skybox, pos);							\n"
	"}                                                                  \n"
	};

	// Load shader source
#if 1
	// Load from string
	glShaderSource( vertexShader, 1, vertexShaderSource, nullptr );
	glShaderSource( fragmentShader, 1, fragmentShaderSource, nullptr );
#else
	// TEST
	// Load from files
	const std::string vertexShaderFilename = "skyBox_vert.glsl";
	std::string vertexShaderFileContent;
	getFileContent( vertexShaderFilename, vertexShaderFileContent );
	const char* sourceCode = vertexShaderFileContent.c_str();
	glShaderSource( vertexShader, 1, &sourceCode, nullptr );
	glShaderSource( fragmentShader, 1, fragmentShaderSource, nullptr );
#endif

	glCompileShader( vertexShader );
	glCompileShader( fragmentShader );

	GLint compileStatus;
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compileStatus );
	if ( compileStatus == GL_FALSE )
	{
		std::cout << "Error: vertex shader "<< std::endl;

		GLint logInfoLength = 0;
		glGetShaderiv( vertexShader, GL_INFO_LOG_LENGTH, &logInfoLength );
		if ( logInfoLength > 0 )
		{
			GLchar* infoLog = new GLchar[ logInfoLength ];
			GLsizei length = 0;
			glGetShaderInfoLog( vertexShader, logInfoLength, &length, infoLog );
			std::cout << infoLog << std::endl;
		}
	}

	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compileStatus );
	if ( compileStatus == GL_FALSE )
	{
		std::cout << "Error: fragment shader "<< std::endl;

		GLint logInfoLength = 0;
		glGetShaderiv( fragmentShader, GL_INFO_LOG_LENGTH, &logInfoLength );
		if ( logInfoLength > 0 )
		{
			GLchar* infoLog = new GLchar[ logInfoLength ];
			GLsizei length = 0;
			glGetShaderInfoLog( fragmentShader, logInfoLength, &length, infoLog );
			std::cout << infoLog << std::endl;
		}
	}

	glAttachShader( mCubeMapShaderProgram, vertexShader );
	glAttachShader( mCubeMapShaderProgram, fragmentShader );

	glLinkProgram( mCubeMapShaderProgram );

	// Check linking status
	GLint linkStatus = 0;
	glGetProgramiv( mCubeMapShaderProgram, GL_LINK_STATUS, &linkStatus );
	if ( linkStatus == GL_FALSE )
	{
		// LOG
		// ...

		GLint logInfoLength = 0;
		glGetProgramiv( mCubeMapShaderProgram, GL_INFO_LOG_LENGTH, &logInfoLength );
		if ( logInfoLength > 0 )
		{
			// Return information log for program object
			GLchar* infoLog = new GLchar[ logInfoLength ];
			GLsizei length = 0;
			glGetProgramInfoLog( mCubeMapShaderProgram, logInfoLength, &length, infoLog );

			// LOG
			std::cout << "\nGsShaderProgram::link() - link ERROR" << std::endl;
			std::cout << infoLog << std::endl;

			delete[] infoLog;
		}

		return false;
	}

	return statusOK;
}

/******************************************************************************
 * Procedural mesh
 ******************************************************************************/
void waves( std::vector< glm::vec3 >& points, std::vector< glm::vec3 >& normals, std::vector< GLuint >& triangleIndices, int nb )
{
	// Position and normal arrays
    points.resize( nb * nb );
    normals.resize( nb * nb );
    for ( int j = 0; j < nb; ++j )
    {
        for ( int i = 0; i < nb; ++i )
        {
			// Current data index
            const int k = j * nb + i;

			// Current position
            float x = 6.0f / nb * j - 3.000001f;
            float y = 6.0f / nb * i - 3.000001f;

			// Position
			// analytic function: sinus (with altitude attenuation)
			// - altitude (use polar coordinates)
            const float r = std::sqrt( x * x + y * y );
//          const float h = 0.4f*std::sin(M_PI/2.0+r*7);
            const float h = 0.4f * ( 1 - r / 5 ) * std::sin( glm::pi< float >() / 2.0 + r * 5 );
			// - store position
//          points[ k ] = { x, y, h };
            points[ k ] = { x, h, y };

			// Normal
			// - derivative of analytic function (use polar coordinates)
			//const float dh = 7*0.4f*std::cos(M_PI/2.0+r*7);
			const float dh = -0.4 / 5 * std::sin( glm::pi< float >() / 2.0 + r * 5 ) + 0.4f * ( 1 - r / 5 ) * 5 * std::cos( glm::pi< float >() / 2.0 + r * 5 );
			// - derivative is the tangent, need to retrieve normal from tangent (easy in 2D)
			const glm::vec3 n = { -x / r * dh, -y / r * dh, 1 };
			// - store normal
            normals[ k ]= glm::normalize( n );
        }
    }
 
	// Index array
    triangleIndices.reserve( 6 * ( nb - 1 ) * ( nb - 1 ) );
    for ( int j = 1; j < nb; ++j )
        for ( int i = 1; i < nb; ++i )
        {
			// Current data index
            const int k = j * nb + i;
			// triangle
            triangleIndices.push_back( k );
            triangleIndices.push_back( k - nb );
            triangleIndices.push_back( k - nb - 1 );
			// triangle
            triangleIndices.push_back( k );
            triangleIndices.push_back( k - nb - 1 );
            triangleIndices.push_back( k - 1 );
        }
}

/******************************************************************************
 * Helper function used to load shader source code from files
 *
 * @param pFilename ...
 *
 * @return ...
 ******************************************************************************/
bool getFileContent( const std::string& pFilename, std::string& pFileContent )
{
	std::ifstream file( pFilename.c_str(), std::ios::in );
	if ( file )
	{
		// Initialize a string to store file content
		file.seekg( 0, std::ios::end );
		pFileContent.resize( file.tellg() );
		file.seekg( 0, std::ios::beg );

		// Read file content
		file.read( &pFileContent[ 0 ], pFileContent.size() );

		// Close file
		file.close();

		return true;
	}
	else
	{
		// LOG
		// ...
	}

	return false;
}

/******************************************************************************
 * Initialize all
 ******************************************************************************/
bool initialize()
{
	std::cout << "Initialize all..." << std::endl;

	bool statusOK = true;

	if ( statusOK )
	{
		statusOK = checkExtensions();
	}

	if ( statusOK )
	{
		statusOK = initializeArrayBuffer();
	}

	if ( statusOK )
	{
		statusOK = initializeVertexArray();
	}

	if ( statusOK )
	{
		statusOK = initializeShaderProgram();
	}

	model.loadModel(objname);
	model.loadTexture(texturename);
	shader->init();

	if ( statusOK )
	{
		statusOK = initializeCubemap();
	}
	
	initializeCamera();

	return statusOK;
}

/******************************************************************************
 * Initialize the camera
 ******************************************************************************/
void initializeCamera()
{
	// User parameters
	// - view
	//_cameraEye = glm::vec3( 0.f, 2.f, 3.f );
	_cameraEye = glm::vec3(0.f, 0.f, 4.f);
	_cameraCenter = glm::vec3( 0.f, 0.f, 0.f );
	_cameraUp = glm::vec3( 0.f, 1.f, 0.f );
	// - projection
	_cameraFovY = 45.f;
	_cameraAspect = 1.f;
	_cameraZNear = 0.1f;
	_cameraZFar = 100.f;
}

/******************************************************************************
 * Finalize all
 ******************************************************************************/
bool finalize()
{
	bool statusOK = true;

	std::cout << "Finalize all..." << std::endl;

	return statusOK;
}

/******************************************************************************
 * Check GL extensions
 ******************************************************************************/
bool checkExtensions()
{
	bool statusOK = true;

	std::cout << "Check GL extensions..." << std::endl;

	return statusOK;
}

/******************************************************************************
 * Initialize array buffer
 ******************************************************************************/
bool initializeArrayBuffer()
{
	bool statusOK = true;

	std::cout << "Initialize array buffer..." << std::endl;

	// In this example, we want to display one triangle

	// Buffer of positions on CPU (host)
	std::vector< glm::vec3 > points;
	std::vector< glm::vec3 > normals;
	std::vector< GLuint > triangleIndices;
	const int nb = 100;
	waves( points, normals, triangleIndices, nb );
	numberOfVertices_ = static_cast< int >( points.size() );
	numberOfIndices_ = static_cast< int >( triangleIndices.size() );

    // Position buffer
	glGenBuffers( 1, &positionBuffer );
    // buffer courant a manipuler
	glBindBuffer( GL_ARRAY_BUFFER, positionBuffer );
    // definit la taille du buffer et le remplit
	glBufferData( GL_ARRAY_BUFFER, numberOfVertices_ * sizeof( glm::vec3 ), points.data(), GL_STATIC_DRAW );
    // buffer courant : rien
    glBindBuffer( GL_ARRAY_BUFFER, 0 );

    // Index buffer
	// - this buffer is used to separate topology from positions: send points + send toplogy (triangle: 3 vertex indices)
    glGenBuffers( 1, &indexBuffer );
    // buffer courant a manipuler
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );
    // definit la taille du buffer et le remplit
    glBufferData( GL_ELEMENT_ARRAY_BUFFER, numberOfIndices_ * sizeof( GLuint ), triangleIndices.data(), GL_STATIC_DRAW );
    // buffer courant : rien
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// Mesh parameter(s)
	_meshColor = glm::vec3( 0.f, 1.f, 0.f );

  	return statusOK;
}

/******************************************************************************
 * Initialize vertex array
 ******************************************************************************/
bool initializeVertexArray()
{
	bool statusOK = true;

	std::cout << "Initialize vertex array..." << std::endl;

	// Create a vertex array to encapsulate all VBO
	// - generate a VAO ID
	glGenVertexArrays( 1, &vertexArray );

	// - bind VAO as current vertex array (in OpenGL state machine)
	glBindVertexArray( vertexArray );
	// - bind VBO as current buffer (in OpenGL state machine)
	glBindBuffer( GL_ARRAY_BUFFER, positionBuffer );

	// - specify the location and data format of the array of generic vertex attributes at index​ to use when rendering
	glVertexAttribPointer( 0/*index of the generic vertex attribute: VBO index (not its ID!)*/, 3/*nb elements in the attribute: (x,y,z)*/, GL_FLOAT/*type of data*/, GL_FALSE/*normalize data*/, 0/*stride*/, 0/*offset in memory*/ );
	// - enable or disable a generic vertex attribute array
	glEnableVertexAttribArray( 0/*index of the generic vertex attribute*/ );

 	// Index buffer
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer );

	// - unbind VAO (0 is the default resource ID in OpenGL)
	glBindVertexArray( 0 );
	// - unbind VBO (0 is the default resource ID in OpenGL)
	glBindBuffer( GL_ARRAY_BUFFER, 0 );

	return statusOK;
}

/******************************************************************************
 * Initialize shader program
 ******************************************************************************/
bool initializeShaderProgram()
{
	bool statusOK = true;

	std::cout << "Initialize shader program..." << std::endl;

	shaderProgram = glCreateProgram();

	GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
	GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

	// Vertex shader
	const char* vertexShaderSource[] = {
//	    "#version 330 core                             \n"
	    "#version 130                                  \n"
		"                                              \n"
		" // INPUT                                     \n"
		" in vec3 position;                            \n"
		"                                              \n"
		" // UNIFORM                                   \n"
		" // - camera                                  \n"
		" uniform mat4 viewMatrix;                     \n"
		" uniform mat4 projectionMatrix;               \n"
		" // - 3D model                                \n"
		" uniform mat4 modelMatrix;                    \n"
		" // - animation                               \n"
		" uniform float time;                          \n"
		"                                              \n"
		" // OUTPUT                                    \n"
	    "                                              \n"
		" // MAIN                                      \n"
	    "void main( void )                             \n"
	    "{                                             \n"
		"#if 1                                                                                  \n"
		"    // Use animation                                                                   \n"
		"    float amplitude = 1.0;                                                             \n"
		"    float frequency = 0.5;                                                             \n"
		"    float height = amplitude * sin( 2.0 * 3.141592 * frequency * ( time * 0.001 ) );   \n"
		"    vec3 pos = vec3( position.x, position.y + height, position.z );                    \n"
		"    // Send position to Clip-space                                                     \n"
		"    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( pos, 1.0 );      \n"
		"#else                                                                                  \n"
		"    // Send position to Clip-space                                                     \n"
	    "    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( position, 1.0 ); \n"
		"#endif                                                                                 \n"
		"}                                                                                      \n"
	};

	// Fragment shader
	const char* fragmentShaderSource[] = {
		"#version 130                                  \n"
		"                                              \n"
		" // INPUT                                     \n"
		"                                              \n"
		" // UNIFORM                                   \n"
		" uniform vec3 meshColor;                      \n"
		"                                              \n"
		" // OUTPUT                                    \n"
	    " out vec4 fragmentColor;                      \n"
	    "                                              \n"
		" // MAIN                                      \n"
		"void main( void )                             \n"
	    "{                                             \n"
	    "    fragmentColor = vec4( meshColor, 1.0 );   \n"
	    "}                                             \n"
	};

	// Load shader source
#if 1
	// Load from string
	glShaderSource( vertexShader, 1, vertexShaderSource, nullptr );
	glShaderSource( fragmentShader, 1, fragmentShaderSource, nullptr );
#else
	// TEST
	// Load from files
	const std::string vertexShaderFilename = "vertexShader.vert";
	std::string vertexShaderFileContent;
	getFileContent( vertexShaderFilename, vertexShaderFileContent );
	const char* sourceCode = vertexShaderFileContent.c_str();
	glShaderSource( vertexShader, 1, &sourceCode, nullptr );
	glShaderSource( fragmentShader, 1, fragmentShaderSource, nullptr );
#endif

	glCompileShader( vertexShader );
	glCompileShader( fragmentShader );

	GLint compileStatus;
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &compileStatus );
	if ( compileStatus == GL_FALSE )
	{
		std::cout << "Error: vertex shader "<< std::endl;

		GLint logInfoLength = 0;
		glGetShaderiv( vertexShader, GL_INFO_LOG_LENGTH, &logInfoLength );
		if ( logInfoLength > 0 )
		{
			GLchar* infoLog = new GLchar[ logInfoLength ];
			GLsizei length = 0;
			glGetShaderInfoLog( vertexShader, logInfoLength, &length, infoLog );
			std::cout << infoLog << std::endl;
		}
	}

	glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &compileStatus );
	if ( compileStatus == GL_FALSE )
	{
		std::cout << "Error: fragment shader "<< std::endl;
	}

	glAttachShader( shaderProgram, vertexShader );
	glAttachShader( shaderProgram, fragmentShader );

	glLinkProgram( shaderProgram );

	return statusOK;
}

/******************************************************************************
 * Callback to display the scene
 ******************************************************************************/
void display( void )
{
	// Timer info
	const int currentTime = glutGet( GLUT_ELAPSED_TIME );

	//--------------------
	// START frame
	//--------------------
	// Clear the color buffer (of the main framebuffer)
	// - color used to clear
	glClearColor( 0.f, 0.f, 0.f, 0.f );
	glClearDepth( 1.f );
	// - clear the "color" framebuffer
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	// Cull triangles which normal is not towards the camera
	glEnable(GL_CULL_FACE);
	// - clear the "color" framebuffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	//--------------------
	// Camera
	//--------------------

	
	// Retrieve camera parameters
	// - view matrix
	//const float pan = 1.8f * sinf( currentTime * 0.0005f );
	//const glm::vec3 cameraFront = glm::rotate( glm::vec3( 0.f, 0.f, -1.f )/*front z axis*/, pan, _cameraUp );
	const glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);
	//_cameraEye = glm::rotate(_cameraEye, pan, _cameraUp);
	//const glm::vec3 cameraFront = glm::rotate( glm::vec3( 0.f, 0.f, -1.f )/*front z axis*/, pan, glm::cross(glm::vec3(0.f, 0.f, -1.f),_cameraUp ));
	const glm::mat4 viewMatrix = glm::lookAt(_cameraEye, cameraFront, _cameraUp);
	// - projection matrix
	const glm::mat4 projectionMatrix = glm::perspective(_cameraFovY, _cameraAspect, _cameraZNear, _cameraZFar);

	/*
	const float pan = 0.03f;
	const glm::vec3 cameraFront = glm::vec3(0.f, 0.f, -1.f);
	_cameraEye = glm::rotate(_cameraEye, pan, _cameraUp);
	const glm::mat4 viewMatrix = glm::lookAt(_cameraEye, cameraFront, _cameraUp);
	const glm::mat4 projectionMatrix = glm::perspective(_cameraFovY, _cameraAspect, _cameraZNear, _cameraZFar);

	*/
	// Tmp variable for GPU uniforms
	GLint uniformLocation;

	//--------------------------------------------------------------------------------
	// Cubemap
	//--------------------------------------------------------------------------------

	// Activation de la cubemap
	// ...
	
	// Set shader program
	glUseProgram( mCubeMapShaderProgram );

	// Model view projection matrix
	uniformLocation = glGetUniformLocation( mCubeMapShaderProgram, "uModelViewProjectionMatrix" );
	if ( uniformLocation >= 0 )
	{
		glm::mat4 modelMatrix = glm::mat4( 1.f );
		const float scale = 10.f; // TODO: modify this to scale your cubemap size !!!!
		modelMatrix = glm::scale( modelMatrix, glm::vec3( scale, scale, scale ) );
		glm::mat4 MVP = projectionMatrix * viewMatrix * modelMatrix;
		glUniformMatrix4fv( uniformLocation, 1/*count*/, GL_FALSE/*transpose*/, glm::value_ptr( MVP ) );
	}

	// Modify GL state(s)
	// ...
	//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
				
	// Draw command
	const GLsizei nbCubemapIndices = 6/*nb faces*/ * 2/*2 triangles per face*/ * 3/*nb indices per triangle*/;
	glBindVertexArray( mCubemapVertexArray );
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	glDrawElements( GL_TRIANGLES/*mode*/, nbCubemapIndices/*count*/, GL_UNSIGNED_INT/*type*/, 0/*indices*/ );
	
	// Reset GL state(s)
	glUseProgram( 0 );
	glBindVertexArray( 0 );
	glBindTexture( GL_TEXTURE_CUBE_MAP, 0 );
		
	// Un-modify GL state(s)
	// ...
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	//--------------------------------------------------------------------------------
	// YOUR SCENE
	//--------------------------------------------------------------------------------

	//--------------------
	// Activate shader program
	//--------------------
	glUseProgram( shaderProgram );

	//--------------------
	// Send uniforms to GPU
	//--------------------
	
	// Retrieve 3D model / scene parameters
	glm::mat4 modelMatrix;

	if ( useMeshAnimation )
	{
		modelMatrix = glm::rotate( modelMatrix, static_cast< float >( currentTime ) * 0.001f, glm::vec3( 0.0f, 1.f, 0.f ) );
	}
	// Camera
	// - view matrix
	uniformLocation = glGetUniformLocation( shaderProgram, "viewMatrix" );
	if ( uniformLocation >= 0 )
	{
		glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, glm::value_ptr( viewMatrix ) );
	}
	// - projection matrix
	uniformLocation = glGetUniformLocation( shaderProgram, "projectionMatrix" );
	if ( uniformLocation >= 0 )
	{
		glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, glm::value_ptr( projectionMatrix ) );
	}
	// Mesh
	// - model matrix
	uniformLocation = glGetUniformLocation( shaderProgram, "modelMatrix" );
	if ( uniformLocation >= 0 )
	{
		glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, glm::value_ptr( modelMatrix ) );
	}
	// - mesh color
	uniformLocation = glGetUniformLocation( shaderProgram, "meshColor" );
	if ( uniformLocation >= 0 )
	{
		glUniform3fv( uniformLocation, 1, glm::value_ptr( _meshColor ) );
	}
	// Animation
	uniformLocation = glGetUniformLocation( shaderProgram, "time" );
	if ( uniformLocation >= 0 )
	{
		glUniform1f( uniformLocation, static_cast< float >( currentTime ) );
	}

	//--------------------
	// Render scene
	//--------------------
	
	// Set GL state(s) (fixed pipeline)
	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	
	/*
	// - bind VAO as current vertex array (in OpenGL state machine)
	glBindVertexArray( vertexArray );
	// - draw command
    glDrawElements(
         GL_TRIANGLES,      // mode
         numberOfIndices_,  // count
         GL_UNSIGNED_INT,   // data type
         (void*)0           // element array buffer offset
    );
	// - unbind VAO (0 is the default resource ID in OpenGL)
	glBindVertexArray( 0 );
	*/
	// Reset GL state(s) (fixed pipeline)
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glUseProgram(shader->programID);

	uniformLocation = glGetUniformLocation(shaderProgram, "viewMatrix");
	if (uniformLocation >= 0)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(viewMatrix));
	}
	// - projection matrix
	uniformLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	if (uniformLocation >= 0)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
	}
	// Mesh
	// - model matrix
	uniformLocation = glGetUniformLocation(shaderProgram, "modelMatrix");
	if (uniformLocation >= 0)
	{
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(modelMatrix));
	}

	uniformLocation = glGetUniformLocation(shader->programID, "meshColor");
	if (uniformLocation >= 0)
		glUniform3f(uniformLocation, 0.f, 0.f, 1.f);

	uniformLocation = glGetUniformLocation(shader->programID, "meshTexture");
	if (uniformLocation >= 0)
		glUniform1i(uniformLocation, 0); // 0 pour GL_TEXTURE0

	// - bind VAO as current vertex array (in OpenGL state machine)
	glBindVertexArray(model.vaoID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, model.texture->texture);

	// Draw the triangles !
	glDrawElements(GL_TRIANGLES, model.indexes.size(), GL_UNSIGNED_INT, (void *)0);

	glBindVertexArray( 0 );
	// Deactivate current shader program
	glUseProgram( 0 );

	//--------------------
	// END frame
	//--------------------
	// OpenGL commands are not synchrone, but asynchrone (stored in a "command buffer")
	glFlush();
	// Swap buffers for "double buffering" display mode (=> swap "back" and "front" framebuffers)
	glutSwapBuffers();
}

/******************************************************************************
 * Callback continuously called when events are not being received
 ******************************************************************************/
void idle( void )
{
	// Mark current window as needing to be redisplayed
	glutPostRedisplay();
}

/******************************************************************************
 * Main function
 ******************************************************************************/

bool endswith(const std::string &str, const std::string &suffix)
{
	return str.size() >= suffix.size() &&
		   str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
}

void keyboard( unsigned char key, int x, int y)
{
	switch(key)
	{
		case 'z':
			_cameraEye = _cameraEye + glm::vec3(0.f, 0.f, -0.1f);
			break;
		case 's':
			_cameraEye = _cameraEye + glm::vec3(0.f, 0.f, 0.1f);
			break;
		case 'q':
			_cameraEye = _cameraEye + glm::vec3(-0.1f, 0.f, 0.f);
			break;
		case 'd':
			_cameraEye = _cameraEye + glm::vec3(0.1f, 0.f, 0.f);
			break;
		case 'Z':
			_cameraEye = _cameraEye + glm::vec3(0.f, 0.1f, 0.f);
			break;
		case 'S':
			_cameraEye = _cameraEye + glm::vec3(0.f, -0.1f, 0.f);
			break;
		case 'Q':
			_cameraEye = glm::rotate(_cameraEye, pan, _cameraUp);

			// pan = pan - 0.1f;
			// if( pan < -1 )
			// 	pan = 0.9f;
			break;
		case 'D':
			_cameraEye = glm::rotate(_cameraEye, -pan, _cameraUp);

			// pan = pan + 0.1f;
			// if( pan > 1 )
			// 	pan = -0.9;
			break;
		case 'p':
			useMeshAnimation = !useMeshAnimation;
		default:
			break;
	}
}


int main( int argc, char** argv )
{
	std::cout << "OpenGL obj display" << std::endl;

	// Retrieve program/data directory
	std::string programPath = argv[ 0 ];
	std::size_t found = programPath.find_last_of( "/\\" );
	dataRepository = programPath.substr( 0, found );
	dataRepository = get_current_dir_name();
	std::cout << "Data Repository: " << dataRepository << std::endl;

	objname = "earth/earth.obj";
	texturename = "earth/4096_earth.jpg";
	std::string vertexShaderFilename = "glsl/vertexShader.vert";
	std::string fragmentShaderFilename = "glsl/fragmentShader.frag";

	for (int i = 1; i < argc; i++)
	{
		if (endswith(argv[i], "obj"))
		{
			objname = argv[i];
			std::cout << "Obj: " << argv[i] << std::endl;
		}
		if (endswith(argv[i], "vert"))
		{
			vertexShaderFilename = argv[i];
			std::cout << "Vertex: " << argv[i] << std::endl;
		}
		if (endswith(argv[i], "frag"))
		{
			fragmentShaderFilename = argv[i];
			std::cout << "Fragment: " << argv[i] << std::endl;
		}
		if (endswith(argv[i], "jpg") || endswith(argv[i], "png") || endswith(argv[i], "PNG") || endswith(argv[i], "tga"))
		{
			texturename = argv[i];
			std::cout << "Texture: " << argv[i] << std::endl;
		}
	}
	shader = new Shader(vertexShaderFilename, fragmentShaderFilename);

	// Initialize the GLUT library
	glutInit( &argc, argv );

	//glutInitContextVersion( 3, 3 );
	//glutInitContextProfile( GLUT_COMPATIBILITY_PROFILE );
	
	// Grahics window
	// - configure the main framebuffer to store rgba colors,
	//   and activate double buffering (for fluid/smooth visualization)
	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE );
	// - window size and position
	glutInitWindowSize( 640, 480 );
	glutInitWindowPosition( 50, 50 );
	// - create the window
	glutCreateWindow( "TP6 - Cubemap" );
	glutFullScreen();

	// Callbacks

	//Keyboard
	glutKeyboardFunc( keyboard );
	// - callback called when displaying window (user custom fonction pointer: "void f( void )")
	glutDisplayFunc( display );
	// - callback continuously called when events are not being received
	glutIdleFunc( idle );

	// Initialize the GLEW library
	// - mandatory to be able to use OpenGL extensions,
	//   because OpenGL core API is made of OpenGL 1 and other functions are null pointers (=> segmentation fault !)
	//   Currently, official OpenGL version is 4.5 (or 4.6)
	GLenum error = glewInit();
	if ( error != GLEW_OK )
	{
		fprintf( stderr, "Error: %s\n", glewGetErrorString( error ) );
		exit( -1 );
	}

	// Initialize all your resources (graphics, data, etc...)
	initialize();

	// Enter the GLUT main event loop (waiting for events: keyboard, mouse, refresh screen, etc...)
	glutMainLoop();

	// Clean all
	//finalize();
}
