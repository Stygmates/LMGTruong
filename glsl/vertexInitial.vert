#version 130
in vec3 position;
out vec3 color;
void main( void )
{
    color = vec3( 0.0, 0.0, 50.0 );
    gl_Position = vec4( position, 1.0 );
}