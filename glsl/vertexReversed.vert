#version 320 es
precision mediump float;
// INPUT
// - vertex attributes
in vec3 position;
in vec2 textureCoordinate;
// UNIFORM
// - animation
uniform float time;
// OUTPUT
out vec2 uv;
// MAIN
void main( void )
{
	// Send position to Clip-space
	gl_Position = vec4( position, 1.0 );
	uv = textureCoordinate;
}
