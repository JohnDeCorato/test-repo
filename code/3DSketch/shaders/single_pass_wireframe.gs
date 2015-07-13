#version 330

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform float aspect;
uniform float wireframe_width;

noperspective out vec3 dist;

void main()
{
	vec2 aspectVec = vec2(aspect, 1.0);

	vec4 p0 = gl_in[0].gl_Position;
	vec4 p1 = gl_in[1].gl_Position;
	vec4 p2 = gl_in[2].gl_Position;

	vec2 p0s = p0.xy / p0.w * aspectVec;
	vec2 p1s = p1.xy / p1.w * aspectVec;
	vec2 p2s = p2.xy / p2.w * aspectVec;

	vec2 v0 = p2s - p1s;
	vec2 v1 = p2s - p0s;
	vec2 v2 = p1s - p0s;

	float area = abs(v1.x * v2.y - v1.y * v2.x);

	dist = vec3(area/length(v0), 0, 0);
	gl_Position = p0;
	EmitVertex();

	dist = vec3(0, area/length(v1), 0);
	gl_Position = p1;
	EmitVertex();

	dist = vec3(0, 0, area/length(v2));
	gl_Position = p2;
	EmitVertex();

	EndPrimitive();
}