#ifndef SHADERPROGRAM_H
#define SHADERPROGRAM_H

#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QOpenGLFunctions>

class ShaderProgram : public QOpenGLShaderProgram, protected QOpenGLFunctions
{
public:
    ShaderProgram();
    ~ShaderProgram();
    void bindAttributeBuffer(QOpenGLBuffer buffer, const char* varName, GLenum type,
                             int tupleSize, int offset);
    template<typename T> void bindBufferData(QOpenGLBuffer buffer, QVector<T> data)
    {
        buffer.bind();
        buffer.allocate(data.constData(), data.size() * sizeof(T));
    }

    virtual void init() = 0;
    virtual void bindBuffers(QVector<QVector3D> verts) = 0;
    virtual void render() = 0;
};

#endif // SHADERPROGRAM_H
