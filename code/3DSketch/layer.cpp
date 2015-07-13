#include "layer.h"
#include <QDebug>

Layer::Layer(string name)
{
    strokes = vector<Stroke*>(0, NULL);
    layerName = name;
    activeStroke = NULL;
}

Stroke* Layer::createNewStroke(QColor color, float lineWidth)
{
    if (!activeStroke)
    {
        activeStroke = new Stroke(color, lineWidth);
        strokes.push_back(activeStroke);
    }
    return activeStroke;
}

int Layer::getStrokeCount()
{
    return strokes.size();
}

void Layer::deleteLastStroke()
{
    strokes.pop_back();
}

//void Layer::drawLayer(QOpenGLShaderProgram *program)
//{
//    for(int i = 0; i < strokes.size(); i++)
//        strokes[i]->drawStroke(program);
//}

vector<Stroke*> Layer::getStrokes()
{
    return strokes;
}

void Layer::addPointToActiveStroke(QVector3D point)
{
    if (activeStroke != NULL)
    {
        activeStroke->addPoint(point);
    }
}

void Layer::endCurrentStroke()
{
    if (activeStroke)
    {
        activeStroke->endStroke();
        activeStroke = NULL;
    }
}
