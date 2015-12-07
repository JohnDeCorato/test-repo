#ifndef VERTEXBASEDLINESHADER_H
#define VERTEXBASEDLINESHADER_H

#include "shaderprogram.h"

class VertexBasedLineShader : public ShaderProgram
{
public:
    VertexBasedLineShader();
    ~VertexBasedLineShader();

    virtual void init();
    virtual void render();
    virtual void bindBuffers(QVector<QVector3D> verts);
    virtual void bindBuffers(QVector<QVector3D> verts, QVector<GLushort> inds);

private:
    // OpenGL Buffers
    QOpenGLBuffer vertBuf;
    QOpenGLBuffer prevBuf;
    QOpenGLBuffer nextBuf;
    QOpenGLBuffer dirBuf;
    QOpenGLBuffer indexBuf;

    // OpenGL Veriables
    int numIndices;
};

#endif // VERTEXBASEDLINESHADER_H
