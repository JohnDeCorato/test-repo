#ifndef STROKE_H
#define STROKE_H

#include <QVector3D>
#include <QVector>
#include <QColor>
#include <QTextStream>

#include <iostream>
#include <fstream>

using namespace std;

class Stroke
{
public:
    Stroke(QColor color = Qt::black, float lineWidth = 0.01);
    virtual ~Stroke();

//    // Render functions
//    void drawStroke(QOpenGLShaderProgram *program);
//    void drawStroke2(QOpenGLShaderProgram *program);

    // Data Management Functions
    void addPoint(QVector3D &point);
    void endStroke();

    QVector<QVector3D> getStroke();
    QVector<QVector3D> getControlPoints();
    QColor getColor();
    float getLineWidth();

private:

//    // OpenGL Buffers
//    QOpenGLBuffer arrayBuf;
//    QOpenGLBuffer vertBuf;
//    QOpenGLBuffer prevBuf;
//    QOpenGLBuffer nextBuf;
//    QOpenGLBuffer dirBuf;
//    QOpenGLBuffer indexBuf;

//    // OpenGL Veriables
//    int numIndices;

    // Stroke Point Containers
    QVector<QVector3D> points;
    QVector<QVector3D> curve;

    // Spline functions for converting a stroke into a B-spline curve
    void buildSpline(QVector<QVector3D> controlPoints);
    void subdivide(QVector3D* points, int id, float epsilon);
    float computeOffsets(QVector3D* points, int id);
    bool madeCurve;

    QColor mColor;
    float lineWidth;
//    // OpenGL Utility Functions
//    void bindBuffers(QVector<QVector3D> verts);
//    void bindAttributeBuffer(QOpenGLBuffer buffer, const char* varName,
//                             QOpenGLShaderProgram *program, GLenum type,
//                             int tupleSize, int offset);
};

inline QColor Stroke::getColor() { return mColor; }
inline float Stroke::getLineWidth() { return lineWidth; }

#endif // STROKE_H
