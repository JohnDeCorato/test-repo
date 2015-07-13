#ifndef RAYCASTER_H
#define RAYCASTER_H

#include <QVector3D>
#include "camera.h"
#include "embree2/rtcore.h"

// Ray Caster Class

//TODO: initialize with model and create acceleration structure
//change raycast code to work with bvh
// Uses Intel's Embree for ray casting
class RayCaster
{
public:
    RayCaster();

    bool unproject(QVector2D screenPos, float winZ, Camera* mCamera, float width, float height, QVector3D &worldCoords);

    QVector3D generateRayDirection(QVector2D screenPos, float width, float height, QMatrix4x4 projection, Camera* mCamera);

    float rayTri(QVector3D& origin, QVector3D& direction, QVector3D& A,
                QVector3D& B, QVector3D& C, float t0, float t1);

    QVector3D* findIntersection(QVector3D& origin, QVector3D& direction,
                               QVector<QVector3D>& triangles);

private:
    RTCScene scene;

};

#endif // RAYCASTER_H
