#ifndef SKETCHMANAGER_H
#define SKETCHMANAGER_H

#include "layer.h"
#include "raycaster.h"
#include "shaderprogram.h"
#include "camera.h"
#include "objloader.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>


using namespace std;

class SketchManager
{
public:
    SketchManager();

    // Save/Load functions
    void loadSketch(ifstream &in);
    void saveSketch(ofstream &out);
    void loadOBJ(std::string fileName);

    // Test functions. Don't call these
    void saveSketch(ofstream &out, Layer* layer);
    Layer* getLayerByName(string name);

    // Sketch Management Functions
    bool stroke(float mouseX, float mouseY, Camera* mCamera, float lineWidth, RayCaster rayCaster, QVector<QVector3D> geometry);
    void endStroke();

    // Layer Management Functions
    bool addLayer(string name);
    void deleteLayer(string name);
    void changeActiveLayer(string name);

    void drawLayers(ShaderProgram* program, Camera* mCamera, QVector<string> renderLayerSet);

private:
    vector<Layer*> layers;
    Layer* activeLayer;
    std::vector<tinyobj::shape_t> geometry;

};

inline vector<string> split(const string &s, char delim)
{
    vector<string> elems;
    stringstream ss(s);
    string item;
    while(getline(ss,item,delim))
    {
        elems.push_back(item);
    }
    return elems;
}

inline Layer* SketchManager::getLayerByName(string name)
{
    return layers[0];
}

#endif // LAYERMANAGER_H
