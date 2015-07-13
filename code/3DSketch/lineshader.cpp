#include "lineshader.h"

LineShader::LineShader()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    positionBuf.create();
    indexBuf.create();
}

LineShader::~LineShader()
{
    positionBuf.destroy();
    indexBuf.destroy();
}

void LineShader::init()
{
    addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/miter_line_shader.vs");
    addShaderFromSourceFile(QOpenGLShader::Geometry, ":/shaders/miter_line_shader.gs");
    addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/miter_line_shader.fs");
    link();
    bind();


}

void LineShader::bindBuffers(QVector<QVector3D> verts)
{
    bind();
    QVector<GLuint> indices;

    // Set up the line adjacency data
    indices.append(0);
    for (int i = 0; i < verts.size(); i++)
    {
        indices.append(i);
    }
    indices.append(verts.size()-1);
    numIndices = indices.size();



    // Bind buffers
    bindBufferData(positionBuf, verts);
    bindBufferData(indexBuf, indices);
}

void LineShader::render()
{
    bind();
    bindAttributeBuffer(positionBuf, "position", GL_FLOAT, 3, sizeof(QVector3D));
    indexBuf.bind();
    glDrawElements(GL_LINE_STRIP_ADJACENCY, numIndices, GL_UNSIGNED_INT, 0);
}

