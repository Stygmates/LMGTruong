#version 130
in vec3 color;
out vec4 fragmentColor;
uniform vec3 meshColor;
void main( void )
{
    fragmentColor = vec4( meshColor, 1.0 );
    //fragmentColor = vec4( color, 1.0 );
}