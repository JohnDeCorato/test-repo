#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;

attribute vec3 position;

//! [0]
void main()
{
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * vec4(position,1.0);
}
