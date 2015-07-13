#ifndef LAYER_H
#define LAYER_H

#include "stroke.h"

using namespace std;

class Layer
{
public:
    Layer(string name = "");

    Stroke* createNewStroke(QColor color = Qt::black, float lineWidth = 0.1);
    void deleteLastStroke();

    vector<Stroke*> getStrokes();
    int getStrokeCount();
    string getName();

    void addPointToActiveStroke(QVector3D point);
    void endCurrentStroke();
//    void drawLayer(QOpenGLShaderProgram *program);

private:
    vector<Stroke*> strokes;

    string layerName;
    Stroke* activeStroke;
};

inline string Layer::getName() { return layerName; }

#endif // LAYER_H
