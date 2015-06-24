#define _USE_MATH_DEFINES
#include "camera.h"
#include <math.h>

const QVector3D Camera::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Camera::LocalRight(1.0f, 0.0f, 0.0f);

Camera::Camera(QVector3D position, QVector3D center, float width, float height)
{
    m_world.lookAt(position, center, LocalUp);
}

// Transform By (Add/Scale)
void Camera::translate(const QVector3D &dt)
{
    m_dirty = true;
    m_translation += dt;
}

void Camera::rotate(const QQuaternion &dr)
{
    m_dirty = true;
    m_rotation = dr * m_rotation;
}

// Transform To (Setters)
void Camera::setTranslation(const QVector3D &t)
{
    m_dirty = true;
    m_translation = t;
}

void Camera::setRotation(const QQuaternion &r)
{
    m_dirty = true;
    m_rotation = r;
}

// Accessors
const QMatrix4x4 &Camera::toMatrix()
{
    return m_world;
}

// Queries
QVector3D Camera::forward() const
{
    return QVector3D(m_world * QVector4D(LocalForward, 0.0f));
}

QVector3D Camera::right() const
{
    return QVector3D(m_world * QVector4D(LocalRight, 0.0f));
}

QVector3D Camera::up() const
{
    return QVector3D(m_world * QVector4D(LocalUp, 0.0f));
}

QVector3D Camera::position()
{
    return QVector3D(m_world*QVector4D(0,0,0,1.0));
}

QMatrix4x4 Camera::fovToPerspective(float fovy, float aspect, float zNear, float zFar)
{
    float top = zNear * tan(fovy * (float)M_PI / 360.0f);
    float bottom = -top;
    float left = bottom * aspect;
    float right = top * aspect;

    QMatrix4x4 mat;
    mat.ortho(left,right,bottom,top, zNear,zFar);
    return mat;
}
