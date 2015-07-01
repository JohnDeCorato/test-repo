#include "layer.h"

Layer::Layer()
{
    strokes = QVector<Stroke*>(0, NULL);
}

Stroke* Layer::createNewStroke()
{
    Stroke* s = new Stroke();
    strokes.append(s);
    return s;
}

int Layer::getStrokeCount()
{
    return strokes.size();
}

void Layer::deleteLastStroke()
{
    strokes.removeLast();
}

void Layer::drawLayer(QOpenGLShaderProgram *program)
{
    for(int i = 0; i < strokes.size(); i++)
        strokes[i]->drawStroke2(program);
}

