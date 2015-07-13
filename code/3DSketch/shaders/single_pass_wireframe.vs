#version 330

uniform mat4 mvp_matrix;
in vec3 position;

void main()
{
	gl_Position = mvp_matrix * vec4(position, 1.0);
}