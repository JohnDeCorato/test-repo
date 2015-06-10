#include "stroke.h"

Stroke::Stroke()
    : indexBuf(QOpenGLBuffer::IndexBuffer)
{
    initializeOpenGLFunctions();

    // Generate VBOs
    arrayBuf.create();
    indexBuf.create();
}

Stroke::~Stroke()
{
    arrayBuf.destroy();
    indexBuf.destroy();
}

// Transfers the stroke data to the VBOS
// TODO: Port over spline functions

void Stroke::endStroke()
{
    arrayBuf.bind();
    arrayBuf.allocate(points.constData(), points.size() * sizeof(QVector3D));

    QVector<GLint> indices;

    for (int i = 0; i < points.size(); i++)
    {
        indices.append(i);
        indices.append(i);
    }

    indexBuf.bind();
    indexBuf.allocate(indices.constData(), 2*points.size() * sizeof(GLushort));
}

// Draw the stroke
void Stroke::drawStroke(QOpenGLShaderProgram *program)
{
    arrayBuf.bind();
    indexBuf.bind();

    // Tell OpenGL pipeline hoe to locate the vertex position
    int vertexLocation = program->attributeLocation("a_position");
    program->enableAttributeArray(vertexLocation);
    program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

    glDrawElements(GL_TRIANGLE_STRIP, 2*points.size(), GL_INT, 0);
}

// Add a point to the stroke
void Stroke::addPoint(QVector3D point)
{
    points.append(point);
}
