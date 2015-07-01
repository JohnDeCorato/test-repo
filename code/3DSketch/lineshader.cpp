#include "lineshader.h"

LineShader::LineShader()
{
    addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/lines.vs");
    addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/lines.fs");
}

