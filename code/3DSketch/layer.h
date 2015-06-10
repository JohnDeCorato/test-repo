#ifndef LAYER_H
#define LAYER_H

#include "stroke.h"

class Layer
{
public:
    Layer();

    Stroke* createNewStroke();
    void deleteLastStroke();
    int getStrokeCount();
    void drawLayer(QOpenGLShaderProgram *program);

private:
    QVector<Stroke*> strokes;
};

#endif // LAYER_H
