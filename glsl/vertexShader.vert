#version 320 es
precision mediump float;
// INPUT
// - vertex attributes
in vec3 position;
in vec2 textureCoordinate;
// UNIFORM

uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
// - animation
uniform float time;
// OUTPUT
out vec2 uv;
// MAIN
void main( void )
{
	// Send position to Clip-space
	gl_Position = vec4( position, 1.0 );
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4( position, 1.0 );
	uv = textureCoordinate;
}