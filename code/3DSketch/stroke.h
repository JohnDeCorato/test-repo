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

    // Render functions
    void drawStroke(QOpenGLShaderProgram *program);
    void drawStroke2(QOpenGLShaderProgram *program);

    // Data Management Functions
    void addPoint(QVector3D &point);
    void endStroke();

private:

    // OpenGL Buffers
    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer vertBuf;
    QOpenGLBuffer prevBuf;
    QOpenGLBuffer nextBuf;
    QOpenGLBuffer dirBuf;
    QOpenGLBuffer indexBuf;

    // OpenGL Veriables
    int numIndices;

    // Stroke Point Containers
    QVector<QVector3D> points;
    QVector<QVector3D> curve;

    // Spline functions for converting a stroke into a B-spline curve
    void buildSpline(QVector<QVector3D> controlPoints);
    void subdivide(QVector3D* points, int id, float epsilon);
    float computeOffsets(QVector3D* points, int id);
    bool madeCurve;

    // OpenGL Utility Functions
    void bindBuffers(QVector<QVector3D> verts);
    void bindAttributeBuffer(QOpenGLBuffer buffer, const char* varName, QOpenGLShaderProgram *program, GLenum type, int tupleSize);
};

#endif // STROKE_H
