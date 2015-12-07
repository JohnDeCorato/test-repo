#ifndef LINESHADER_H
#define LINESHADER_H

#include "shaderprogram.h"


class LineShader : public ShaderProgram
{
public:
    LineShader();
    ~LineShader();

    virtual void init();
    virtual void render();
    virtual void bindBuffers(QVector<QVector3D> verts);
    virtual void bindBuffers(QVector<QVector3D> verts, QVector<GLushort> inds);

private:
    QOpenGLBuffer positionBuf;
    QOpenGLBuffer indexBuf;

    int numIndices;
};

#endif // LINESHADER_H
