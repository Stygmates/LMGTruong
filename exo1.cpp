//Compilation : g++ -std=c++11 -o exo1 exo1.cpp -lGL -lGLU -lglut -lGLEW

/******************************************************************************
 ******************************* INCLUDE SECTION ******************************
 ******************************************************************************/

// STL
#include <iostream>
#include <vector>
#include <sstream>
#include <string>

// System
#include <cstdio>

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

#include "My3DModel.hpp"

/******************************************************************************
 ****************************** NAMESPACE SECTION *****************************
 ******************************************************************************/

const char* vertexShaderSource[] = {
"#version 130											\n"
"in vec3 position;										\n"
"out vec3 color;										\n"
"void main( void )										\n"
"{														\n"
"color = vec3( 0.0, 0.0, 50.0 );						\n"
"gl_Position = vec4( position, 1.0 );					\n"
"}														\n"
};

const char* fragmentShaderSource[] = {
"#version 130											\n"
"in vec3 color;											\n"
"out vec4 fragmentColor;								\n"
"uniform vec3 meshColor;								\n"
"void main( void )										\n"
"{														\n"
"//fragmentColor = vec4( meshColor, 1.0 );				\n"
"fragmentColor = vec4( color, 1.0 );					\n"
"}														\n"
};	

/******************************************************************************
 ************************* DEFINE AND CONSTANT SECTION ************************
 ******************************************************************************/

// VBO (vertex buffer object) : used to store positions coordinates at each point
//GLuint positionBuffer;
// VAO (vertex array object) : used to encapsulate several VBO
//GLuint vertexArray;

My3DModel model;

// Program id
GLuint programID ;

//Path to the obj
std::string objname;

/******************************************************************************
 ***************************** TYPE DEFINITION ********************************
 ******************************************************************************/

/******************************************************************************
***************************** METHOD DEFINITION ******************************
******************************************************************************/

/******************************************************************************
 ***************************** METHOD DEFINITION ******************************
 ******************************************************************************/

bool checkShader( GLuint shader );
bool checkShaderLinking(GLuint programID);
bool initialize();
bool checkExtensions();
bool initializeArrayBuffer();
bool initializeVertexArray();
bool initializeShaderProgram();
bool finalize();


/******************************************************************************
 * Errors check
 ******************************************************************************/

bool checkShader( GLuint shader )
{
	// Check compilation status
	GLint compileStatus;
	glGetShaderiv( shader, GL_COMPILE_STATUS, &compileStatus );
	if ( compileStatus == GL_FALSE )
	{
		GLint logInfoLength = 0;
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &logInfoLength );
		if ( logInfoLength > 0 )
		{
			// Return information log for shader object
			GLchar* infoLog = new GLchar[ logInfoLength ];
			GLsizei length = 0;
			glGetShaderInfoLog( shader, logInfoLength, &length, infoLog );
			// LOG
			std::cout << infoLog << std::endl;
		}
		return false;
	}
	return true;
}

/*bool checkShaderLinking( GLuint programID )
{
	// Check linking status
	GLint linkStatus = 0;
	glGetProgramiv( _program, GL_LINK_STATUS, &linkStatus );
	if ( linkStatus == GL_FALSE )
	{
		GLint logInfoLength = 0;
		glGetProgramiv( _program, GL_INFO_LOG_LENGTH, &logInfoLength );
		if ( logInfoLength > 0 )
		{
			// Return information log for program object
			GLchar* infoLog = new GLchar[ logInfoLength ];
			GLsizei length = 0;
			glGetProgramInfoLog( _program, logInfoLength, &length, infoLog );
			// LOG
			std::cout << infoLog << std::endl;
		}
		return false;
	}
}
*/

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

	model.loadModel( objname );
	//model.displayVertexBuffers();
	if ( statusOK )
	{
		statusOK = initializeShaderProgram();
	}

	return statusOK;
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

	/*

	std::cout << "Initialize array buffer..." << std::endl;

	// In this example, we want to display one triangle

	// Buffer of positions on CPU (host)
	// - BEWARE: counter-clock wise !
	// IMPORTANT: here, data have to be placed in NDC space (normalized device coordinates system),
	//            i.e. in a cube from [-1;-1] (in front of screen) to [1;1] (behind screen)
	// Data is stored as (x,y,z) triplets: (x1,y1,z1,x2,y2,z2,x3,y3,z3)
	const std::vector< float > positions = {
	-1.f, -1.f, 0.f,
	 1.f, -1.f, 0.f,
	 0.f,  1.f, 0.f
	};

	// Create a GPU buffer (device)
	// - generate a VBO ID
	glGenBuffers( 1, &positionBuffer );
	// - bind VBO as current buffer (in OpenGL state machine)
	glBindBuffer( GL_ARRAY_BUFFER, positionBuffer );
	// - send data from CPU (host) to GPU (device)
	glBufferData( GL_ARRAY_BUFFER, positions.size() * sizeof( float ), positions.data(), GL_STATIC_DRAW );
	// - unbind VBO (0 is the default resource ID in OpenGL)
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	*/
	return statusOK;
}

/******************************************************************************
 * Initialize vertex array
 ******************************************************************************/
bool initializeVertexArray()
{
	bool statusOK = true;
	
	// // std::cout << "Initialize vertex array..." << std::endl;

	// // // Create a vertex array to encapsulate all VBO
	// // // - generate a VAO ID
	// // glGenVertexArrays( 1, &vertexArray );

	// // // - bind VAO as current vertex array (in OpenGL state machine)
	// // glBindVertexArray( vertexArray );
	// // // - bind VBO as current buffer (in OpenGL state machine)
	// // glBindBuffer( GL_ARRAY_BUFFER, positionBuffer );

	// // // - specify the location and data format of the array of generic vertex attributes at index​ to use when rendering
	// // glVertexAttribPointer( 0/*index of the generic vertex attribute: VBO index (not its ID!)*/, 3/*nb elements in the attribute: (x,y,z)*/, GL_FLOAT/*type of data*/, GL_FALSE/*normalize data*/, 0/*stride*/, 0/*offset in memory*/ );
	// // // - enable or disable a generic vertex attribute array
	// // glEnableVertexAttribArray( 0/*index of the generic vertex attribute*/ );

	// // // - unbind VAO (0 is the default resource ID in OpenGL)
	// // glBindVertexArray( 0 );
	// // // - unbind VBO (0 is the default resource ID in OpenGL)
	// glBindBuffer( GL_ARRAY_BUFFER, 0 );
	return statusOK;
}

/******************************************************************************
 * Initialize shader program
 ******************************************************************************/
bool initializeShaderProgram()
{
	bool statusOK = true;

	std::cout << "Initialize shader program..." << std::endl;
	//Create the ids of the shaders
	GLuint vertexShaderID = glCreateShader( GL_VERTEX_SHADER );
	GLuint fragmentShaderID = glCreateShader( GL_FRAGMENT_SHADER );
	//
	glShaderSource( vertexShaderID, 1, vertexShaderSource, nullptr );
	glShaderSource( fragmentShaderID, 1, fragmentShaderSource, nullptr );
	
	//Compile the shaders
	glCompileShader( vertexShaderID );
	if( !checkShader( vertexShaderID ) )
		return false;

	glCompileShader( fragmentShaderID );
	if( !checkShader( fragmentShaderID ) )
		return false;

	//Create the program
	programID = glCreateProgram();
	//Attach the shades to the program
	glAttachShader( programID, vertexShaderID );
	glAttachShader( programID, fragmentShaderID );
	//Link the program
	glLinkProgram( programID );

	return statusOK;
}

/******************************************************************************
 * Callback to display the scene
 ******************************************************************************/
void display( void )
{
	std::cout << "Callback: display..." << std::endl;

	// ...
	//glViewport( 0, 0, 640, 480 );

	// Clear the color buffer (of the main framebuffer)
	// - color used to clear
	glClearColor( 0.f, 0.f, 0.f, 0.f );
	glClearDepth( 1.f );
	// - clear the "color" framebuffer
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glUseProgram( programID );

	GLint uniformLocation = glGetUniformLocation( programID, "meshColor");
	glUniform3f(uniformLocation, 1.f, 0.f, 0.f);

	
	// Render user custom data
	// - for this example, store a default common color for all points (in OpenGL state machine)
	//glColor3f( 1.f, 0.f, 0.f );
	// - bind VAO as current vertex array (in OpenGL state machine)
	glBindVertexArray( model.vertexArray );
	// - render primitives from array data (here interpreted as primitives of type "triangles")
	//   => pass the first index of points and their numbers (1 triangle made of 3 points)
	//glDrawArrays( GL_POINTS, 0, model.getNbVertices() );

	// Index buffer
	
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, model.indexBuffer );
	// Draw the triangles !
	glDrawElements( GL_TRIANGLES, model.indexes.size(), GL_UNSIGNED_INT, (void*)0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );

	// - unbind VAO (0 is the default resource ID in OpenGL)
	glBindVertexArray( 0 );

	glUseProgram( 0 );

	// OpenGL commands are not synchrone, but asynchrone (stored in a "command buffer")
	glFlush();
	// Swap buffers for "double buffering" display mode (=> swap "back" and "front" framebuffers)
	glutSwapBuffers();
}

/******************************************************************************
 * Main function
 ******************************************************************************/
int main( int argc, char** argv )
{
	std::cout << "Exo 1 - OpenGL with GLUT" << std::endl;
	std::stringstream s;
	if( argc == 2 )
		s << argv[1];
	else
		s << "obj/cube.obj";
	objname = s.str();

	// Initialize the GLUT library
	glutInit( &argc, argv );

		// - configure the main framebuffer to store rgba colors,
		//   and activate double buffering (for fluid/smooth visualization)
		glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	// - window size and position
	glutInitWindowSize( 1920, 1080 );
	glutInitWindowPosition( 50, 50 );
	// - create the window
	glutCreateWindow( "Exo 1" );
	// - Initialize fullscreen
	glutFullScreen();
	// - callback when displaying window (user custom fonction pointer: "void f( void )")
	glutDisplayFunc( display );

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
}
