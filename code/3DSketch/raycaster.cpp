#define _USE_MATH_DEFINES
#include "raycaster.h"

#include <QVector2D>
#include <QVector3D>
#include <QMatrix4x4>
#include <limits>
#include <math.h>

// Simple port of the basic RayCaster Operations
// TODO: Convert BVH Tree Code
RayCaster::RayCaster()
{

}

bool RayCaster::unproject(QVector2D screenPos, float winZ, Camera *mCamera, float width, float height, QVector3D &worldCoords)
{
    QMatrix4x4 inverse = mCamera->getMVPMatrix().inverted();

    float winX = screenPos.x();
    float winY = height - screenPos.y();

    QVector4D in;
    in[0] = winX / width  * 2.0f - 1.0f;
    in[1] = winY / height * 2.0f - 1.0f;
    in[2] = 2.0f * winZ - 1.0f;
    in[3] = 1;

    QVector4D out = inverse * in;
    if (out[3] == 0.0)
    {
        worldCoords = QVector3D(0,0,0);
        return (false);
    }

    out[3] = 1 / out[3];
    worldCoords = QVector3D(out.x(),out.y(),out.z()) * out[3];
    return true;
}

QVector3D RayCaster::generateRayDirection(QVector2D screenPos, float width, float height, QMatrix4x4 projection, Camera* mCamera)
{
    QVector3D p0, p1;
    unproject(screenPos, 0.0f, mCamera, width, height, p0);
    unproject(screenPos, 1.0f, mCamera, width, height, p1);

    return (p1 - p0).normalized();
}

float RayCaster::rayTri(QVector3D& origin, QVector3D& direction, QVector3D& A,
                       QVector3D& B, QVector3D& C, float t0, float t1)
{
    float a = A.x() - B.x();
    float b = A.y() - B.y();
    float c = A.z() - B.z();

    float d = A.x() - C.x();
    float e = A.y() - C.y();
    float f = A.z() - C.z();

    float g = direction.x();
    float h = direction.y();
    float i = direction.z();

    float j = A.x() - origin.x();
    float k = A.y() - origin.y();
    float l = A.z() - origin.z();

    float eimhf = e*i-h*f;
    float gfmdi = g*f-d*i;
    float dhmeg = d*h-e*g;
    float akmjb = a*k-j*b;
    float jcmal = j*c-a*l;
    float blmkc = b*l-k*c;

    float M = a*eimhf+b*gfmdi+c*dhmeg;

    // Degenerate case

    if (M == 0)
        return -1;

    // Solve for t


    float t = -(f*akmjb+e*jcmal+d*blmkc)/M;

    if (t < t0 || t > t1)
        return -1.0f;

    float gamma = (i*akmjb+h*jcmal+g*blmkc)/M;

    if (gamma < 0 || gamma > 1)
        return -1.0f;

    float beta = (j*eimhf+k*gfmdi+l*dhmeg)/M;

    if (beta < 0 || beta > 1)
        return -1;

    float alpha = 1 - beta - gamma;

    if (alpha < 0)
        return -1;

    return t - 0.05;
}

QVector3D* RayCaster::findIntersection(QVector3D& origin, QVector3D& direction,
                                      QVector<QVector3D>& triangles)
{
    float t0 = 1.0f;
    float t1 = std::numeric_limits<float>::infinity();

    // Brute force test
    // TODO: Change to acceleration structure
    for (int i = 0; i < triangles.size()/3; i++)
    {
        float t = rayTri(origin, direction, triangles[3*i], triangles[3*i+1], triangles[3*i+2], t0, t1);
        if (t != -1)
        {
            t1 = t;
        }
    }

    // If we hit anything
    if (t1 != std::numeric_limits<float>::infinity())
    {
        return new QVector3D(origin + t1 * direction);
    }
    return NULL;
}

