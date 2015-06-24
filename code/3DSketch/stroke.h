#ifndef STROKE_H
#define STROKE_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>

class Stroke : protected QOpenGLFunctions
{
public:
    Stroke();
    virtual ~Stroke();

    void drawStroke(QOpenGLShaderProgram *program);
    void addPoint(QVector3D &point);
    void endStroke();

private:

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QVector<QVector3D> points;

    QVector<QVector3D> curve;

    // Spline functions for converting a stroke into a B-spline curve
    void buildSpline(QVector<QVector3D> controlPoints);
    void subdivide(QVector3D* points, int id, float epsilon);
    float computeOffsets(QVector3D* points, int id);
    bool madeCurve;
};

#endif // STROKE_H
