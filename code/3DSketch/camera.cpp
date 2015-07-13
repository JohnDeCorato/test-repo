#define _USE_MATH_DEFINES
#include "camera.h"
#include <math.h>

const QVector3D Camera::LocalForward(0.0f, 0.0f, -1.0f);
const QVector3D Camera::LocalUp(0.0f, 1.0f, 0.0f);
const QVector3D Camera::LocalRight(1.0f, 0.0f, 0.0f);

Camera::Camera(QVector3D position, QVector3D center, float width, float height,
               float fovy, float zNear, float zFar)
{
    m_world.lookAt(position, center, LocalUp);

    mWidth = width;
    mHeight = height;
    mFOVy = fovy;
    mZNear = zNear;
    mZFar = zFar;

    mCameraPosition = position;
    mLookAtPosition = center;
    mUpVector = QVector3D(0,1,0);

    setModelMatrix(QVector3D(1,0,0),0,QVector3D(0,0,0),QVector3D(1,1,1));
    setProjectionMatrix(mWidth, mHeight);
    setViewMatrix(mCameraPosition, mLookAtPosition, mUpVector);

    totalZoom = position.distanceToPoint(center);
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
    return QVector3D(mViewMatrix * QVector4D(LocalForward, 0.0f));
}

QVector3D Camera::right() const
{
    return QVector3D(mViewMatrix * QVector4D(LocalRight, 0.0f));
}

QVector3D Camera::up() const
{
    return QVector3D(mViewMatrix * QVector4D(LocalUp, 0.0f));
}

QVector3D Camera::position()
{
    return mCameraPosition;
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

// Replacement Camera Functions for hnadling movenet though mouse
// TODO: Clean up most of the above code
void Camera::translate(float dx, float dy)
{
    QVector3D cameraW = (mLookAtPosition - mCameraPosition).normalized();
    QVector3D cameraU = QVector3D::crossProduct(mUpVector, cameraW).normalized();

    QVector3D changel = mLookAtPosition + mUpVector * (10.0 * dy / mWidth)
            + cameraU * (10.0 * dx / mHeight);
    QVector3D changec = mCameraPosition + mUpVector * (10.0 * dy / mWidth)
            + cameraU * (10.0 * dx / mHeight);

    mCameraPosition = changec;
    mLookAtPosition = changel;

    setViewMatrix(mCameraPosition, mLookAtPosition, mUpVector);
}

void Camera::rotate(float dx, float dy)
{
    float roty = 1.5f * dx / mWidth;
    float rotx = 1.5f * dy / mHeight;
    float phi = rotx;
    float theta = roty;

    QVector3D cameraW = (mLookAtPosition - mCameraPosition).normalized();
    QVector3D cameraU = QVector3D::crossProduct(cameraW, mUpVector).normalized();

    QQuaternion q = (QQuaternion(theta, QVector3D(0.0, 1.0, 0.0)) * QQuaternion(phi, cameraU)).normalized();
    QMatrix4x4 rotMat = QMatrix4x4();
    rotMat.rotate(q);

    QVector3D tmp = mCameraPosition - mLookAtPosition;
    QVector4D myPoint = QVector4D(tmp, 1.0);
    QVector4D myVector = QVector4D(mUpVector);

    QVector4D newPos = rotMat * myPoint;
    QVector4D tmp1 = rotMat * myVector;

    // Hacky fix for some degeneracy
    newPos.setY(-newPos.y());
    tmp1.setY(-tmp1.y());

    mCameraPosition = QVector3D(newPos) + mLookAtPosition;
    mUpVector = QVector3D(tmp1).normalized();
    setViewMatrix(mCameraPosition, mLookAtPosition, mUpVector);
}

void Camera::zoom(float d)
{
    mRealDiv = mWidth + mHeight;
    QVector3D dir = (mCameraPosition - mLookAtPosition).normalized();

    totalZoom += 9.0 * d / mRealDiv;
    totalZoom = std::max(totalZoom, 0.1f);
    QVector3D change2 = mLookAtPosition + dir * totalZoom;

    mCameraPosition = change2;
    setViewMatrix(mCameraPosition, mLookAtPosition, mUpVector);

}

// Model Matrix Setters
void Camera::setModelMatrix(QVector3D &axis, float angle, QVector3D translate, QVector3D scale)
{
    mModelMatrix = QMatrix4x4();
}

void Camera::setModelMatrix(QQuaternion &r, QVector3D translate, QVector3D scale)
{
    mModelMatrix = QMatrix4x4();
}

// View Matrix Setter
void Camera::setViewMatrix(QVector3D eye, QVector3D lookAt, QVector3D up)
{
    mViewMatrix.setToIdentity();
    mViewMatrix.lookAt(eye, lookAt, up);
}

// Projection Matrix Setter
void Camera::setProjectionMatrix(float width, float height)
{
    mWidth = width;
    mHeight = height;

    mProjectionMatrix.setToIdentity();
    mProjectionMatrix.perspective(mFOVy, width / height, mZNear, mZFar);
}
