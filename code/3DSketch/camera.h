#ifndef CAMERA_H
#define CAMERA_H

#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>

class Camera
{
public:

    // Constants
    static const QVector3D LocalForward;
    static const QVector3D LocalUp;
    static const QVector3D LocalRight;

    // Constructors
    Camera(QVector3D position, QVector3D center, float width, float height, float fovy, float zNear, float zFar);

    // Transform By
    void translate(const QVector3D &dt);
    void translate(float dx, float dy, float dz);
    void rotate(const QQuaternion &dr);
    void rotate(float angle, const QVector3D &axis);
    void rotate(float angle, float ax, float ay, float az);

    // Transform To (Setters)
    void setTranslation(const QVector3D &t);
    void setTranslation(float x, float y, float z);
    void setRotation(const QQuaternion &r);
    void setRotation(float angle, const QVector3D &axis);
    void setRotation(float angle, float ax, float ay, float az);

    // Accessors
    const QVector3D& translation() const;
    const QQuaternion& rotation() const;
    const QMatrix4x4& toMatrix();

    // Camera Transformation Functions Based on Mouse Movement
    void translate(float dx, float dy);
    void rotate(float dx, float dy);
    void zoom(float d);

    // Matrix Setters
    void setModelMatrix(QVector3D &axis, float angle, QVector3D translate, QVector3D scale);
    void setModelMatrix(QQuaternion &r, QVector3D translate, QVector3D scale);
    void setViewMatrix(QVector3D eye, QVector3D lookAt, QVector3D up);
    void setProjectionMatrix(float width, float height);

    // Matrix Getters
    QMatrix4x4 getModelMatrix();
    QMatrix4x4 getViewMatrix();
    QMatrix4x4 getProjectionMatrix();
    QMatrix4x4 getMVPMatrix();

    // Other Getters
    float getWidth();
    float getHeight();
    float getAspectRatio();
    float getFOVy();
    float getZNear();
    float getZFar();

    // Other Camera Functions
    QMatrix4x4 fovToPerspective(float fovy, float aspect, float zNear, float zFar);

    // Queries
    QVector3D forward() const;
    QVector3D right() const;
    QVector3D up() const;
    QVector3D position();

private:
    // True if camera is changed
    bool m_dirty;
    QVector3D m_translation;
    QQuaternion m_rotation;
    QMatrix4x4 m_world;

    float mWidth, mHeight, mFOVy, mZNear, mZFar, mRealDiv, totalZoom;

    QVector3D mCameraPosition;
    QVector3D mLookAtPosition;
    QVector3D mUpVector;

    QMatrix4x4 mModelMatrix;
    QMatrix4x4 mViewMatrix;
    QMatrix4x4 mProjectionMatrix;
};

// Transform By (Add/Scale)
inline void Camera::translate(float dx, float dy,float dz) { translate(QVector3D(dx, dy, dz)); }
inline void Camera::rotate(float angle, const QVector3D &axis) { rotate(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Camera::rotate(float angle, float ax, float ay,float az) { rotate(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Transform To (Setters)
inline void Camera::setTranslation(float x, float y, float z) { setTranslation(QVector3D(x, y, z)); }
inline void Camera::setRotation(float angle, const QVector3D &axis) { setRotation(QQuaternion::fromAxisAndAngle(axis, angle)); }
inline void Camera::setRotation(float angle, float ax, float ay, float az) { setRotation(QQuaternion::fromAxisAndAngle(ax, ay, az, angle)); }

// Accessors
inline const QVector3D& Camera::translation() const { return m_translation; }
inline const QQuaternion& Camera::rotation() const { return m_rotation; }

// Matrix Getters
inline QMatrix4x4 Camera::getModelMatrix() { return mModelMatrix; }
inline QMatrix4x4 Camera::getViewMatrix() { return mViewMatrix; }
inline QMatrix4x4 Camera::getProjectionMatrix() { return mProjectionMatrix; }
inline QMatrix4x4 Camera::getMVPMatrix() { return mProjectionMatrix * mViewMatrix * mModelMatrix; }

// Other Getters
inline float Camera::getWidth() { return mWidth; }
inline float Camera::getHeight() { return mHeight; }
inline float Camera::getAspectRatio() { return mWidth / mHeight; }
inline float Camera::getFOVy() { return mFOVy; }
inline float Camera::getZNear() { return mZNear; }
inline float Camera::getZFar() { return mZFar; }

#endif // CAMERA_H
