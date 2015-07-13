#include "singlepasswireframeshader.h"

SinglePassWireframeShader::SinglePassWireframeShader()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    positionBuf.create();
    indexBuf.create();
}

SinglePassWireframeShader::~SinglePassWireframeShader()
{
    positionBuf.destroy();
    indexBuf.destroy();
}

void SinglePassWireframeShader::init()
{
    addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/single_pass_wireframe.vs");
    addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/single_pass_wireframe.gs");
    addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/single_pass_wireframe.fs");
    link();
    bind();
}

// Default behavior, assume passed in as triangle list w/o index data
void SinglePassWireframeShader::bindBuffers(QVector<QVector3D> verts)
{
    bind();

    QVector<GLshort> indices;
    for (int i = 0; i < verts.size(); i++)
    {
        indices.append(i);
    }

    // Bind buffers
    bindBufferData(positionBuf, verts);
    bindBufferData(indexBuf, indices);

    numIndices = indices.size();
}

void SinglePassWireframeShader::render()
{
    bind();

    bindAttributeBuffer(positionBuf, "position", GL_FLOAT, 3, sizeof(QVector3D));
    indexBuf.bind();
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
}
