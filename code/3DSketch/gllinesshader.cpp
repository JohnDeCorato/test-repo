#include "gllinesshader.h"

GLLinesShader::GLLinesShader()
    :indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    positionBuf.create();
    indexBuf.create();
}

GLLinesShader::~GLLinesShader()
{
    positionBuf.destroy();
    indexBuf.destroy();
}

void GLLinesShader::init()
{
    addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vs");
    addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fs");
    link();
    bind();

}

void GLLinesShader::bindBuffers(QVector<QVector3D> verts)
{
    bindBufferData(positionBuf, verts);

    QVector<GLuint> indices;
    indices.clear();
    for (int i = 0; i < verts.size()-1; i++)
    {
        indices.append(i);
        indices.append(i+1);
    }

    bindBufferData(indexBuf, indices);

    numIndices = indices.size();
}

void GLLinesShader::bindBuffers(QVector<QVector3D> verts, QVector<GLushort> inds)
{

}

void GLLinesShader::render()
{
    bind();

    bindAttributeBuffer(positionBuf, "position", GL_FLOAT, 3, sizeof(QVector3D));
    indexBuf.bind();

    glLineWidth(10);
    glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_INT, 0);
}

