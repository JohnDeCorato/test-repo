#include "stroke.h"
#include <algorithm>
#include <QDebug>


Stroke::Stroke(QColor color, float width)
{
//    initializeOpenGLFunctions();

//    // Generate VBOs
//    arrayBuf.create();
//    vertBuf.create();
//    prevBuf.create();
//    nextBuf.create();
//    dirBuf.create();
//    indexBuf.create();

    mColor = color;
    lineWidth = width;
    madeCurve = false;
}

Stroke::~Stroke()
{
//    arrayBuf.destroy();
//    vertBuf.destroy();
//    prevBuf.destroy();
//    nextBuf.destroy();
//    dirBuf.destroy();
//    indexBuf.destroy();
}

// Transfers the stroke data to the VBOS
// TODO: Port over spline functions

void Stroke::endStroke()
{
    buildSpline(points);
    //curve = points;
//    qDebug() << curve.size();

//    arrayBuf.bind();

//    arrayBuf.allocate(curve.constData(), curve.size() * sizeof(QVector3D));

//    QVector<GLushort> indices;
//    indices.clear();
//    for (int i = 0; i < curve.size()-1; i++)
//    {
//        indices.append(i);
//        indices.append(i+1);
//    }
//    indexBuf.bind();
//    indexBuf.allocate(indices.constData(), indices.size() * sizeof(GLushort));
//    bindBuffers(points);
    madeCurve = true;
}

// Draw the stroke
//void Stroke::drawStroke(QOpenGLShaderProgram *program)
//{

//    indexBuf.bind();

//    bindAttributeBuffer(arrayBuf, "position", program, GL_FLOAT, 3, sizeof(QVector3D));

//    //int vertexLocation = program->attributeLocation("a_position");
//    //program->enableAttributeArray(vertexLocation);
//    //program->setAttributeBuffer(vertexLocation, GL_FLOAT, 0, 3, sizeof(QVector3D));

//    glLineWidth(10);
//    if (!madeCurve)
//        glDrawElements(GL_LINES, 2*(points.size()-1), GL_UNSIGNED_SHORT, 0);
//    else
//        glDrawElements(GL_LINES, 2*(curve.size()-1), GL_UNSIGNED_SHORT, 0);
//}

// Add a point to the stroke
void Stroke::addPoint(QVector3D &point)
{
    points.append(point);
//    arrayBuf.bind();

//    arrayBuf.allocate(points.constData(), points.size() * sizeof(QVector3D));

//    QVector<GLushort> indices;
//    indices.clear();
//    for (int i = 0; i < points.size()-1; i++)
//    {
//        indices.append(i);
//        indices.append(i+1);
//    }
//    indexBuf.bind();
//    indexBuf.allocate(indices.constData(), indices.size() * sizeof(GLushort));
//    bindBuffers(points);
}

// Build a B-Spline curve from a set of control points
void Stroke::buildSpline(QVector<QVector3D> controlPoints)
{
    if (controlPoints.size() < 5)
    {
        curve = controlPoints;
        return;
    }

    int L = controlPoints.size()-3;
    QVector3D* bezier = new QVector3D[3*L+1];
    curve.clear();

    bezier[0] = controlPoints[0];
    bezier[1] = controlPoints[1];
    bezier[2] = (controlPoints[1] + controlPoints[2]) * 0.5f;
    bezier[3*L] = controlPoints[L+2];
    bezier[3*L-1] = controlPoints[L+1];
    bezier[3*L-2] = (controlPoints[L] + controlPoints[L+1]) * 0.5f;

    for (int i = 2; i < L; i++)
    {
        bezier[3*i-2] = (controlPoints[i]*2.0f + controlPoints[i+1])/3.0f;
        bezier[3*i-1] = (controlPoints[i] + controlPoints[i+1]*2.0f)/3.0f;
    }

    for (int i = 1; i < L; i++)
    {
        bezier[3*i] = (bezier[3*i-1] + bezier[3*i+1])*0.5f;
    }

    for (int i = 0; i < L; i++)
    {
        subdivide(bezier, 3*i, 0.0005f);
    }
    curve.append(bezier[3*L]);
    //curve = controlPoints;
    delete bezier;
}

void Stroke::subdivide(QVector3D *points, int id, float epsilon)
{
    if (computeOffsets(points, id) < epsilon)
    {
        for (int i = 0; i < 3; i++)
        {
            curve.append(points[id+i]);
        }
    }
    else
    {
        QVector3D npoints[7];

        npoints[0] = points[id];
        npoints[1] = (points[id] + points[id+1])*0.5f;
        npoints[2] = 0.25f*(points[id] + 2.0f*points[id+1] + points[id+2]);
        npoints[3] = 0.125f*(points[id]+3*points[id+1]+3*points[id+2]+points[id+3]);
        npoints[4] = 0.25f*(points[id+1] + 2.0f*points[id+2] + points[id+3]);
        npoints[5] = (points[id+2] + points[id+3])*0.5f;
        npoints[6] = points[id+3];

        subdivide(npoints,0,epsilon);
        subdivide(npoints,3,epsilon);
        //delete npoints;
    }
}

// Given points P0, P1, P2, and P3, computes the max distance between
// points P1 or P2 to the line segment [P0 P3]
float Stroke::computeOffsets(QVector3D *points, int id)
{
    QVector3D v = points[id+3] - points[id];
    QVector3D w = points[id+1] - points[id];

    float S1;
    float c1 = QVector3D::dotProduct(v,w);
    float c2 = QVector3D::dotProduct(v,v);
    if (c1 <= 0)
    {
        S1 = points[id+1].distanceToPoint(points[id+0]);
    }
    else if (c2 <= c1)
    {
        S1 = points[id+1].distanceToPoint(points[id+3]);
    }
    else
    {
        float b = c1 / c2;
        QVector3D Pb = points[id] + v*b;
        S1 = points[id+1].distanceToPoint(Pb);
    }

    v = points[id+3] - points[id];
    w = points[id+2] - points[id];

    float S2;
    c1 = QVector3D::dotProduct(v,w);
    c2 = QVector3D::dotProduct(v,v);
    if (c1 <= 0)
    {
        S2 = points[id+2].distanceToPoint(points[id+0]);
    }
    else if (c2 <= c1)
    {
        S2 = points[id+2].distanceToPoint(points[id+3]);
    }
    else
    {
        float b = c1 / c2;
        QVector3D Pb = points[id] + v*b;
        S2 = points[id+1].distanceToPoint(Pb);
    }

    return std::max(S1,S2);
}

//// Creates and binds all of the data needed to create a rendered line
//void Stroke::bindBuffers(QVector<QVector3D> verts)
//{
//    QVector<QVector3D> vertices;
//    QVector<QVector3D> prevVertices;
//    QVector<QVector3D> nextVertices;
//    QVector<float> directions;
//    QVector<GLshort> indices;



//    for (int i = 0; i < verts.size(); i++)
//    {
//        // First Copy
//        vertices.append(verts[i]);
//        prevVertices.append(verts[std::max(i-1,0)]);
//        nextVertices.append(verts[std::min(i+1,verts.size()-1)]);
//        directions.append(1.0f);
//        indices.append(2*i);

//        // Second copy
//        vertices.append(verts[i]);
//        prevVertices.append(verts[std::max(i-1,0)]);
//        nextVertices.append(verts[std::min(i+1,verts.size()-1)]);
//        directions.append(-1.0f);
//        indices.append(2*i+1);
//    }

//    numIndices = indices.size();

//    // Bind the buffers
//    vertBuf.bind();
//    vertBuf.allocate(vertices.constData(), numIndices * sizeof(QVector3D));

//    prevBuf.bind();
//    prevBuf.allocate(prevVertices.constData(), numIndices * sizeof(QVector3D));

//    nextBuf.bind();
//    nextBuf.allocate(nextVertices.constData(), numIndices * sizeof(QVector3D));

//    dirBuf.bind();
//    dirBuf.allocate(directions.constData(), numIndices * sizeof(float));

//    indexBuf.bind();
//    indexBuf.allocate(indices.constData(), numIndices * sizeof(GLshort));
//}

//// Utility Function to Bind and OpenGL Attribute Buffer
//void Stroke::bindAttributeBuffer(QOpenGLBuffer buffer, const char *varName, QOpenGLShaderProgram *program, GLenum type, int tupleSize, int offset)
//{
//    buffer.bind();
//    int location = program->attributeLocation(varName);
//    program->enableAttributeArray(location);
//    program->setAttributeBuffer(location, type, 0, tupleSize, offset);
//}

//void Stroke::drawStroke2(QOpenGLShaderProgram *program)
//{
//    bindAttributeBuffer(vertBuf, "position", program, GL_FLOAT, 3, sizeof(QVector3D));
//    bindAttributeBuffer(prevBuf, "previous", program, GL_FLOAT, 3, sizeof(QVector3D));
//    bindAttributeBuffer(nextBuf, "next", program, GL_FLOAT, 3, sizeof(QVector3D));
//    bindAttributeBuffer(dirBuf, "direction", program, GL_FLOAT, 1, sizeof(float));

//    indexBuf.bind();
//    glDrawElements(GL_TRIANGLE_STRIP, numIndices, GL_UNSIGNED_SHORT, 0);
//}

QVector<QVector3D> Stroke::getStroke()
{
    return madeCurve ? curve : points;
}

QVector<QVector3D> Stroke::getControlPoints()
{
    return points;
}
