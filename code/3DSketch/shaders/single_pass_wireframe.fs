#version 330

uniform vec3 color;

uniform float wireframe_width;

noperspective in vec3 dist;

out vec4 fragColor;

void main()
{
	float nearD = min(min(dist[0], dist[1]), dist[2]);
	float edgeIntensity = exp2(-1.0 * nearD * nearD);
	//float edgeIntensity = 1 - pow(nearD, 0.1);

	if (nearD < wireframe_width)
		fragColor = vec4(0.5,0.5,0.5,1);
	else 
		fragColor = vec4(color, 1.0);
	//fragColor = vec4(0,0,1,1);
}