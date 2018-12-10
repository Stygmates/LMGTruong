#version 320 es
precision mediump float;

// INPUT
in vec2 uv;
// UNIFORM
// - mesh
uniform vec3 meshColor;
// - animation
//Texture
uniform sampler2D meshTexture;

// OUTPUT
out vec4 fragmentColor;
// MAIN
void main( void )
{
    fragmentColor = texture( meshTexture, vec2(uv.s, 1.0 - uv.t ) );
    //fragmentColor = texture( meshTexture, uv );
}
