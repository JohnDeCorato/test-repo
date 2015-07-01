layout(lines_adjacency) in;
layout(triangle_strip, max_vertices = 18) out;

uniform mat4 mvp_matrix;
uniform float aspect;

uniform float thickness;

vec2 perp(vec2 p)
{
	return vec2(-p.y, p.x);
}

float signed_area(vec2 p1, vec2 p2, vec2 p3)
{
	return (p2.x-p1.x)*(p3.y-p1.y) - (p3.x-p1.x)*(p2.y-p1.y);
}

// return the intersection of two 2D line segments along with an identifier int
vec2 intersect(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
	float denom  = (p4.y-p3.y) * (p2.x-p1.x) - (p4.x-p3.x) * (p2.y-p1.y);
	float numera = (p4.x-p3.x) * (p1.y-p3.y) - (p4.y-p3.y) * (p1.x-p3.x);
	float numerb = (p2.x-p1.x) * (p1.y-p3.y) - (p2.y-p1.y) * (p1.x-p3.x);

	if ( (-eps < numera && numera < eps) &&
            (-eps < numerb && numerb < eps) &&
            (-eps < denom  && denom  < eps) ) {
		return (p1 + p2) * 0.5; //meaning the lines coincide
	}

	if (-eps < denom  && denom  < eps) {
		return vec2(0,0);
	}

	mu = numera / denom;

	return p1 + mua * (p2 - p1);
}

void main(void)
{
	vec2 aspectVec = vec2(aspect, 1.0);

	// The points in the line adjacency transformed into screen space

	vec4 p0 = mvp_matrix * vec4(gl_in[0].gl_Position, 1.0);
	vec4 p1 = mvp_matrix * vec4(gl_in[1].gl_Position, 1.0);
	vec4 p2 = mvp_matrix * vec4(gl_in[2].gl_Position, 1.0);
	vec4 p3 = mvp_matrix * vec4(gl_in[3].gl_Position, 1.0);

	vec2 p0_s = p0.xy / p0.w * aspectVec;
	vec2 p1_s = p1.xy / p1.w * aspectVec;
	vec2 p2_s = p2.xy / p2.w * aspectVec;
	vec2 p3_s = p3.xy / p3.w * aspectVec;

	// Screen Space Line Segment Vectors
	vec2 l0_s = p1_s - p0_s;
	vec2 l1_s = p2_s - p1_s;
	vec2 l2_s = p3_s - p2_s;

	// Screen Space Line Segment Normals
	vec2 n0 = normalize(vec2(-l0_s.y,l0_s.x));
	vec2 n1 = normalize(vec2(-l1_s.y,l1_s.x));
	vec2 n2 = n1;
	vec2 n3 = normalize(vec2(-l2_s.y,l2_s.x));

	if (signed_area(p0,p1,p2) > 0)
	{
		n0 = -n0;
		n1 = -n1;
	}

	if (signed_area(p1,p2,p3) > 0)
	{
		n2 = -n2;
		n3 = -n3;
	}

	vec2 miter_vec1 = intersect(n0+p0,n0+p1,n1+p2.n1+p1) - p1;
	vec2 miter_vec2 = intersect(n2+p1,n2+p2,n3+p3,n3+p2) - p2;

	vec2 midpoint = (p1 + p2) * 0.5;

	float len = thickness;

	//
	// TRIANGULIZATION CODE: See Chapter 5, Line Rendering, for more details
	//

	// First half of the line segment
	if (p0_s != p1_s)
	{
		vec2 vP1 = p1 + miter_vec1 * len;
		vec2 vP2 = p1 - miter_vec1 * len;
		vec2 tA = p1 + n0 * len;
		vec2 tB = p1 + n1 * len;
		vec2 tM = (tA + tB) * 0.5;

		// Not Degenerate: Compute Join
		if (length(vP1 - vP2) < 2*len)
		{
			gl_Position = midpoint + n1 * len;
			EmitVertex();

			gl_Position = midPoint - n1 * len;
			EmitVertex();

			gl_Position = tB;
			EmitVertex();

			gl_Position = vP2;
			EmitVertex();

			gl_position = tM;
			EmitVertex();
			EndPrimitive();
		}
		// Degenerate Case: Use Bevel and Rectangle
		else
		{
			gl_Position = midpoint + n1 * len;
			EmitVertex();

			gl_Position = midPoint - n1 * len;
			EmitVertex();

			gl_Position = tB;
			EmitVertex();

			gl_Position = p1 - n1 * len;
			EmitVertex();

			gl_position = tM;
			EmitVertex();
			EndPrimitive();
		}


	}
	// Endpoint. Just use a rectangle for now, will add endcap support soon
	else
	{
		gl_Position = midpoint + n1 * len;
		EmitVertex();

		gl_Position = midPoint - n1 * len;
		EmitVertex();

		gl_Position = p1 + n1 * len;
		EmitVertex();

		gl_Position = p1 - n1 * len;
		EmitVertex();
		EndPrimitive();
	}

	// Second half of the segment
	if (p2 != p3)
	{
		vec2 vP1 = p2 + miter_vec1 * len;
		vec2 vP2 = p2 - miter_vec1 * len;
		vec2 tA = p2 + n2 * len;
		vec2 tB = p2 + n3 * len;
		vec2 tM = (tA + tB) * 0.5;

		// Not Degenerate: Compute Join
		if (length(vP1 - vP2) < 2*len)
		{
			gl_Position = midpoint + n2 * len;
			EmitVertex();

			gl_Position = midPoint - n2 * len;
			EmitVertex();

			gl_Position = tB;
			EmitVertex();

			gl_Position = vP2;
			EmitVertex();

			gl_position = tM;
			EmitVertex();
			EndPrimitive();
		}
		// Degenerate Case: Use Bevel and Rectangle
		else
		{
			gl_Position = midpoint + n2 * len;
			EmitVertex();

			gl_Position = midPoint - n2 * len;
			EmitVertex();

			gl_Position = tB;
			EmitVertex();

			gl_Position = p2 - n2 * len;
			EmitVertex();

			gl_position = tM;
			EmitVertex();
			EndPrimitive();
		}
	}
	// Endpoint. Just use a rectangle for now, will add endcap support soon
	else
	{
		gl_Position = midpoint + n2 * len;
		EmitVertex();

		gl_Position = midPoint - n2 * len;
		EmitVertex();

		gl_Position = p2 + n2 * len;
		EmitVertex();

		gl_Position = p2 - n2 * len;
		EmitVertex();
		EndPrimitive();
	}


}