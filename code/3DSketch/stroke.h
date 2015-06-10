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
    void addPoint(QVector3D point);
    void endStroke();

private:

    QOpenGLBuffer arrayBuf;
    QOpenGLBuffer indexBuf;
    QVector<QVector3D> points;
};

#endif // STROKE_H
