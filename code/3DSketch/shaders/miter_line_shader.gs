#version 330
#define M_PI 3.14159
#define EPSILON 0.00047

layout(lines_adjacency) in;
layout(triangle_strip, max_vertices = 400) out;

// Projection Matrices
uniform mat4 mvp_matrix;
uniform mat4 mMat;
uniform mat4 vMat;
uniform mat4 pMat;

uniform float aspect;

uniform float thickness;

uniform int stroke_number;

// Join Type: 0 = Miter + Bevel, 1 = Bevel, 2 = Round
uniform int join_type;

///////////////////////////////////////////////////////
// Helper Functions for All Joins
///////////////////////////////////////////////////////

// Returns a perpendicular 2D vector
vec2 perp(vec2 p)
{
	return vec2(-p.y, p.x);
}

float signed_area(vec2 p1, vec2 p2, vec2 p3)
{
	return (p2.x-p1.x)*(p3.y-p1.y) - (p3.x-p1.x)*(p2.y-p1.y);
}

// Return the intersection of the lines defined by (p1,p2) and (p3,p4)
vec2 intersect(vec2 p1, vec2 p2, vec2 p3, vec2 p4)
{
	float eps = 0.000000000001;

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

	float mu = numera / denom;

	return p1 + mu * (p2 - p1);
}

//////////////////////////////////////////////
// Miter Helper Functions
//////////////////////////////////////////////

vec2 computeMiter(vec2 p0, vec2 p1, vec2 p2)
{
	vec2 dirA = normalize(p1-p0);
	vec2 dirB = normalize(p2-p1);

	vec2 tangent = normalize(dirA + dirB);
	vec2 per = perp(dirA);
	vec2 mit = perp(tangent);

	return (thickness * 0.5 / dot(mit, per)) * mit;
}

/////////////////////////////////////////////
// Round Helper Functions
/////////////////////////////////////////////

// Outputs a list of direction vectors with center p going halfway from v1 to v2
// This code assumes v1 and v2 are normalized
vec2[5] calculate_arc(vec2 v1, vec2 v2)
{
	float r = thickness / 2.0;
	float angle1 = acos(v1.x);
	float angle2 = acos(v2.x);

	bool incremental = true;

	if ( v1.y > 0)
	{ 
		angle1 = 2*M_PI - angle1;
	}
	if ( v2.y > 0)
	{
		angle2 = 2*M_PI - angle2;
	}

	if (angle2 > angle1)
	{
		if (angle2 - angle1 > M_PI)
		{
			angle2 = angle2 - 2 * M_PI;
		}
	}
	else
	{
		if (angle1 - angle2 > M_PI)
		{
			angle1 = angle1 - 2 * M_PI;
		}
	}

	if (angle1 > angle2)
	{
		incremental = false;
	}

	float increment = abs((angle2 - angle1) / 4.0);

	vec2[5] output;

	if (incremental)
	{
		for (int i = 0; i < 5; i++)
		{
			float a = angle1 + i * increment;
			output[i] = vec2(cos(a) * r, sin(a) * r);
		}
	}
	else
	{
		for (int i = 0; i < 5; i++)
		{
			float a = angle1 - i * increment;
			output[i] = vec2(cos(a) * r, sin(a) * r);
		}
	}
	return output;
}


/////////////////////////////////////////////
// Endcaps
/////////////////////////////////////////////
void make_endcap(vec4 p, vec2 normal, vec2 direction)
{
	vec2 aspInv = vec2(1/aspect, 1.0);

	vec4 n = vec4(normal * thickness / 2.0 * aspInv, 0, 0);
	vec4 d = vec4(direction * thickness / 2.0 * aspInv, 0, 0);

	gl_Position = p + n;
	EmitVertex();
	gl_Position = p - n;
	EmitVertex();
	gl_Position = p + n + d;
	EmitVertex();
	gl_Position = p - n + d;
	EmitVertex();
	EndPrimitive();
}


/////////////////////////////////////////////
// Main
/////////////////////////////////////////////
void main()
{
	vec2 aspectVec = vec2(aspect, 1.0);
	vec2 aspInv = vec2(1/aspect, 1.0);

	// The points in the line adjacency transformed into screen space
	mat4 adjPMat = pMat;
	//adjPMat[3][3] += 10 * EPSILON;

	vec4 p0 = adjPMat * vMat * mMat * gl_in[0].gl_Position;
	vec4 p1 = adjPMat * vMat * mMat * gl_in[1].gl_Position;
	vec4 p2 = adjPMat * vMat * mMat * gl_in[2].gl_Position;
	vec4 p3 = adjPMat * vMat * mMat * gl_in[3].gl_Position;

	vec2 p0_s = p0.xy / p0.w * aspectVec;
	vec2 p1_s = p1.xy / p1.w * aspectVec;
	vec2 p2_s = p2.xy / p2.w * aspectVec;
	vec2 p3_s = p3.xy / p3.w * aspectVec;

	// Screen Space Line Segment Vectors
	vec2 l0_s = p1_s - p0_s;
	vec2 l1_s = p2_s - p1_s;
	vec2 l2_s = p3_s - p2_s;

	// Screen Space Line Segment Normals
	vec2 n0 = normalize(perp(l0_s));
	vec2 n1 = normalize(perp(l1_s));
	vec2 n2 = n1;
	vec2 n3 = normalize(perp(l2_s));

	if (signed_area(p0_s,p1_s,p2_s) > 0)
	{
		n0 = -n0;
		n1 = -n1;
	}

	if (signed_area(p1_s,p2_s,p3_s) > 0)
	{
		n2 = -n2;
		n3 = -n3;
	}

	float len = thickness;


	n0 *= len/2.0;
	n1 *= len/2.0;
	n2 *= len/2.0;
	n3 *= len/2.0;

	vec2 miter_vec1 = computeMiter(p0_s,p1_s,p2_s);
	vec2 miter_vec2 = computeMiter(p1_s,p2_s,p3_s);

	if (signed_area(p0_s,p1_s,p2_s) > 0)
		miter_vec1 = -miter_vec1;

	if (signed_area(p1_s,p2_s,p3_s) > 0)
		miter_vec2 = -miter_vec2;

	vec4 midpoint = (p1 + p2) * 0.5;

	

	//
        // TRIANGULIZATION CODE: See Chapter 6, Curve Rendering, for more details
	//

	// First half of the line segment
	if (p0_s != p1_s && miter_vec1 != vec2(0,0))
	{
		vec4 vP1 = p1 + vec4(miter_vec1 * aspInv, 0, 0);
		vec4 vP2 = p1 - vec4(miter_vec1 * aspInv, 0, 0);
		vec4 tA = p1 + vec4(n0 * aspInv, 0, 0);
		vec4 tB = p1 + vec4(n1 * aspInv, 0, 0);
		vec4 tM = (tA + tB) * 0.5;

		if (join_type < 2)
		{
			gl_Position = midpoint + vec4(n1 * aspInv, 0, 0);
			EmitVertex();

			gl_Position = midpoint - vec4(n1 * aspInv, 0, 0);
			EmitVertex();

			gl_Position = tB;
			EmitVertex();

			gl_Position = p1 - vec4(n1 * aspInv, 0, 0);
			EmitVertex();

			gl_Position = tM;
			EmitVertex();
			EndPrimitive();

			// Not Degenerate: Compute Join
			if (length(miter_vec1) < len && join_type == 0)
			{
				gl_Position = tB;
				EmitVertex();
				gl_Position = tM;
				EmitVertex();
				gl_Position = vP1;
				EmitVertex();
				EndPrimitive();
			}
		}
		// Round Join
		else
		{
			gl_Position = midpoint + vec4(n1 * aspInv, 0, 0);
			EmitVertex();

			gl_Position = midpoint - vec4(n1 * aspInv, 0, 0);
			EmitVertex();

			gl_Position = tB;
			EmitVertex();

			gl_Position = p1 - vec4(n1 * aspInv, 0, 0);
			EmitVertex();

			gl_Position = p1;
			EmitVertex();
			EndPrimitive();

			vec2 v1 = normalize(n1);
			vec2 v2 = normalize(miter_vec1);

			if (true)
			{
				vec2[5] arc_vectors = calculate_arc(n1, miter_vec1);

				for(int i = 0; i < 4; i++)
				{
					gl_Position = p1 + vec4(arc_vectors[i] * aspInv, 0, 0);
					EmitVertex();

					gl_Position = p1;
					EmitVertex();

					gl_Position = p1 + vec4(arc_vectors[i+1] * aspInv, 0, 0);
					EmitVertex();
					EndPrimitive();
				}
			}
			else
			{
				gl_Position = tB;
				EmitVertex();
				gl_Position = p1;
				EmitVertex();
				gl_Position = tM;
				EmitVertex();
				EndPrimitive();
			}
		}


	}
	// Endpoint. Just use a rectangle for now, will add endcap support soon
	else
	{
		gl_Position = midpoint + vec4(n1 * aspInv, 0, 0);
		EmitVertex();

		gl_Position = midpoint - vec4(n1* aspInv, 0, 0);
		EmitVertex();

		gl_Position = p1 + vec4(n1 * aspInv, 0, 0);
		EmitVertex();

		gl_Position = p1 - vec4(n1 * aspInv, 0, 0);
		EmitVertex();
		EndPrimitive();

		make_endcap(p1, normalize(n1), normalize(-l1_s));
	}

	// Second half of the segment
	if (p2_s != p3_s && miter_vec2 != vec2(0,0))
	{
		vec4 vP1 = p2 + vec4(miter_vec2 * aspInv, 0, 0);
		vec4 tA = p2 + vec4(n2 * aspInv,0,0);
		vec4 tB = p2 + vec4(n3 * aspInv,0,0);
		vec4 tM = (tA + tB) * 0.5;

		if (join_type < 2)
		{
			gl_Position = midpoint + vec4(n2 * aspInv, 0, 0);
			EmitVertex();

			gl_Position = midpoint - vec4(n2 * aspInv, 0, 0);
			EmitVertex();

			gl_Position = tA;
			EmitVertex();

			gl_Position = p2 - vec4(n2 * aspInv,0,0);;
			EmitVertex();

			gl_Position = tM;
			EmitVertex();
			EndPrimitive();

			// Not Degenerate: Compute Join
			if (length(miter_vec2) < len && join_type == 0)
			{
				gl_Position = tA;
				EmitVertex();

				gl_Position = tM;
				EmitVertex();

				gl_Position = vP1;
				EmitVertex();
				EndPrimitive();
			}
		}
		// Round Join
		else
		{
			gl_Position = midpoint + vec4(n2 * aspInv, 0, 0);
			EmitVertex();

			gl_Position = midpoint - vec4(n2 * aspInv, 0, 0);
			EmitVertex();

			gl_Position = tA;
			EmitVertex();

			gl_Position = p2 - vec4(n2 * aspInv,0,0);;
			EmitVertex();

			gl_Position = p2;
			EmitVertex();
			EndPrimitive();

			vec2 v1 = normalize(n2);
			vec2 v2 = normalize(miter_vec2);

			if (true)
			{
				vec2[5] arc_vectors = calculate_arc(v1, v2);

				for(int i = 0; i < 4; i++)
				{
					gl_Position = p2 + vec4(arc_vectors[i] * aspInv, 0, 0);
					EmitVertex();

					gl_Position = p2;
					EmitVertex();

					gl_Position = p2 + vec4(arc_vectors[i+1] * aspInv, 0, 0);
					EmitVertex();
					EndPrimitive();
				}
			}
			else
			{
				gl_Position = tA;
				EmitVertex();
				gl_Position = p2;
				EmitVertex();
				gl_Position = tM;
				EmitVertex();
				EndPrimitive();
			}
		}
	}
	// Endpoint. Just use a rectangle for now, will add endcap support soon
	else
	{
		gl_Position = midpoint + vec4(n2 * aspInv, 0, 0);
		EmitVertex();

		gl_Position = midpoint - vec4(n2 * aspInv, 0, 0);
		EmitVertex();

		gl_Position = p2 + vec4(n2 * aspInv, 0, 0);
		EmitVertex();

		gl_Position = p2 - vec4(n2 * aspInv, 0, 0);
		EmitVertex();
		EndPrimitive();

		make_endcap(p1, normalize(n2), normalize(l1_s));
	}

	EndPrimitive();
}
