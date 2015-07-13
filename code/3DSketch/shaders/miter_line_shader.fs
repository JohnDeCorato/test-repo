#version 330

uniform vec4 color;

out vec4 fragColor;

void main()
{
    // Set fragment color from texture
    fragColor = color;
}