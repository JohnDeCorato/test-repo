#include "shaderprogram.h"

ShaderProgram::ShaderProgram()
{
    initializeOpenGLFunctions();
}

ShaderProgram::~ShaderProgram()
{

}

void ShaderProgram::bindAttributeBuffer(QOpenGLBuffer buffer, const char *varName, GLenum type,
                                   int tupleSize, int offset)
{
    buffer.bind();
    int location = attributeLocation(varName);
    enableAttributeArray(location);
    setAttributeBuffer(location, type, 0, tupleSize, offset);
}
