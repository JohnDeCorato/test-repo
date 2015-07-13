attribute vec3 position;
attribute float direction;
attribute vec3 next;
attribute vec3 previous;

uniform mat4 mvp_matrix;
uniform float aspect;

uniform float thickness;
uniform int miter;

void main()
{
    vec2 aspectVec = vec2(aspect, 1.0);

    // Transform the line points into the distorted image space
    vec4 previousProjected = mvp_matrix * vec4(previous, 1.0);
    vec4 currentProjected = mvp_matrix * vec4(position, 1.0);
    vec4 nextProjected = mvp_matrix * vec4(next, 1.0);

    // Get the 2D screen space positions
    vec2 previousScreen = previousProjected.xy / previousProjected.w * aspectVec;
    vec2 currentScreen = currentProjected.xy / currentProjected.w * aspectVec;
    vec2 nextScreen = nextProjected.xy / nextProjected.w * aspectVec;

    float len = thickness;
    float orientation = direction;

    //starting point uses (next - current)
    vec2 dir = vec2(0.0);
    if (currentScreen == previousScreen)
    {
        dir = normalize(nextScreen - currentScreen);
    }
    //ending point uses (current - previous)
    else if (currentScreen == nextScreen)
    {
        dir = normalize(currentScreen - previousScreen);
    }
    //somewhere in middle, needs a join
    else
    {
        //get directions from (C - B) and (B - A)
        vec2 dirA = normalize((currentScreen - previousScreen));
        if (miter == 1)
        {
            vec2 dirB = normalize((nextScreen - currentScreen));
            //now compute the miter join normal and length
            vec2 tangent = normalize(dirA + dirB);
            vec2 perp = vec2(-dirA.y, dirA.x);
            vec2 miter = vec2(-tangent.y, tangent.x);
            dir = tangent;
            len = thickness / dot(miter, perp);
        }
        else {
            dir = dirA;
        }
    }
    vec2 normal = vec2(-dir.y, dir.x);
    normal *= len/2.0;
    normal.x /= aspect;

    vec4 offset = vec4(normal * orientation, 0.0, 0.0);
    gl_Position = currentProjected + offset;
    gl_PointSize = 1.0;
}
