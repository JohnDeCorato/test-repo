#include "vertexbasedlineshader.h"

VertexBasedLineShader::VertexBasedLineShader()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    vertBuf.create();
    prevBuf.create();
    nextBuf.create();
    dirBuf.create();
    indexBuf.create();
}

VertexBasedLineShader::~VertexBasedLineShader()
{
    vertBuf.destroy();
    prevBuf.destroy();
    nextBuf.destroy();
    dirBuf.destroy();
    indexBuf.destroy();
}

void VertexBasedLineShader::init()
{
    addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/lines.vs");
    addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/lines.fs");
    link();
    bind();
}

void VertexBasedLineShader::bindBuffers(QVector<QVector3D> verts)
{
    QVector<QVector3D> vertices;
    QVector<QVector3D> prevVertices;
    QVector<QVector3D> nextVertices;
    QVector<float> directions;
    QVector<GLuint> indices;



    for (int i = 0; i < verts.size(); i++)
    {
        // First Copy
        vertices.append(verts[i]);
        prevVertices.append(verts[std::max(i-1,0)]);
        nextVertices.append(verts[std::min(i+1,verts.size()-1)]);
        directions.append(1.0f);
        indices.append(2*i);

        // Second copy
        vertices.append(verts[i]);
        prevVertices.append(verts[std::max(i-1,0)]);
        nextVertices.append(verts[std::min(i+1,verts.size()-1)]);
        directions.append(-1.0f);
        indices.append(2*i+1);
    }

    numIndices = indices.size();

    // Bind the buffers
    bindBufferData(vertBuf, vertices);
    bindBufferData(prevBuf, prevVertices);
    bindBufferData(nextBuf, nextVertices);
    bindBufferData(dirBuf, directions);
    bindBufferData(indexBuf, indices);
}

void VertexBasedLineShader::render()
{
    bind();

    bindAttributeBuffer(vertBuf, "position", GL_FLOAT, 3, sizeof(QVector3D));
    bindAttributeBuffer(prevBuf, "previous", GL_FLOAT, 3, sizeof(QVector3D));
    bindAttributeBuffer(nextBuf, "next", GL_FLOAT, 3, sizeof(QVector3D));
    bindAttributeBuffer(dirBuf, "direction", GL_FLOAT, 1, sizeof(float));

    indexBuf.bind();
    glDrawElements(GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_INT, 0);
}
