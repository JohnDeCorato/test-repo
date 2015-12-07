#ifndef GLLINESSHADER_H
#define GLLINESSHADER_H

#include "shaderprogram.h"


class GLLinesShader : public ShaderProgram
{
public:
    GLLinesShader();
    ~GLLinesShader();

    virtual void init();
    virtual void render();
    virtual void bindBuffers(QVector<QVector3D> verts);
    virtual void bindBuffers(QVector<QVector3D> verts, QVector<GLushort> inds);

private:
    QOpenGLBuffer positionBuf;
    QOpenGLBuffer indexBuf;

    int numIndices;
};

#endif // GLLINESSHADER_H
