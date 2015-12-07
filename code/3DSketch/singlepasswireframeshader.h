#ifndef SINGLEPASSWIREFRAMESHADER_H
#define SINGLEPASSWIREFRAMESHADER_H

#include "shaderprogram.h"

class SinglePassWireframeShader : public ShaderProgram
{
public:
    SinglePassWireframeShader();
    ~SinglePassWireframeShader();

    virtual void init();
    virtual void render();
    virtual void bindBuffers(QVector<QVector3D> verts);
    virtual void bindBuffers(QVector<QVector3D> verts, QVector<GLushort> inds);

private:
    QOpenGLBuffer positionBuf;
    QOpenGLBuffer indexBuf;

    int numIndices;
};

#endif // SINGLEPASSWIREFRAMESHADER_H
