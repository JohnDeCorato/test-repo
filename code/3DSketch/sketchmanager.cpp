#include "sketchmanager.h"

SketchManager::SketchManager()
{

}

// Function for saving the sketch to a file
///////////////////////
/// OUTPUT FILE FORMAT
///////////////////////
// l for a new layer
// layer name
// stroke count
// s for new stroke
// line width
// number of control points
// list stroke control points in x y z format
//
///////////////////
/// Example
///////////////////
// l
// layer name
// 2
// s
// 0.1
// 1
// x y z
// s
// 0.5
// 2
// x y z
// x y z
// l
// layer 2
void SketchManager::saveSketch(ofstream & out)
{
    // Save each layer
    for(int i = 0; i < (int) layers.size(); i++)
    {
        out << "l\n";
        Layer* layer = layers[i];
        out << layer->getName() <<"\n";

        std::vector<Stroke*> strokes = layer->getStrokes();
        out << strokes.size() << "\n";

        // Save each stroke
        for(int j = 0; j < (int)strokes.size(); j++)
        {
            out << "s\n";
            out << strokes[j]->getLineWidth() << "\n";
            QVector<QVector3D> controlPoints = strokes[j]->getControlPoints();
            out << controlPoints.size() << "\n";

            // Save each control point
            for (int k = 0; k < controlPoints.size(); k++)
            {
                QVector3D p = controlPoints[k];
                out << p.x() << " " << p.y() << " " << p.z() << "\n";
            }
        }

    }
}

// Test function with old structure. Don't call this
void SketchManager::saveSketch(ofstream & out, Layer *layer)
{
    out << "l\n";
    out << layer->getName() << "\n";

    std::vector<Stroke*> strokes = layer->getStrokes();
    out << strokes.size() << "\n";

    // Save each stroke
    for(int j = 0; j < (int)strokes.size(); j++)
    {
        out << "s\n";
        out << strokes[j]->getLineWidth() << "\n";
        QVector<QVector3D> controlPoints = strokes[j]->getControlPoints();
        out << controlPoints.size() << "\n";

        // Save each control point
        for (int k = 0; k < controlPoints.size(); k++)
        {
            QVector3D p = controlPoints[k];
            out << p.x() << " " << p.y() << " " << p.z() << "\n";
        }
    }
}


// Load the sketch with the file format described above
void SketchManager::loadSketch(ifstream &in)
{
    layers.clear();
    string line;
    if(in.is_open())
    {
        while(getline(in,line))
        {
            if (line=="l")
            {
                // Create the layer with it's name
                getline(in, line);
                qDebug() << "Layer Name: " << line.c_str();
                bool t=  addLayer(line);


                // Get the number of strokes
                getline(in, line);
                qDebug() << line.c_str();
                int numStrokes = stoi(line);

                qDebug() << "Number of strokes: " << numStrokes;

                for (int i = 0; i < numStrokes; i++)
                {
                    getline(in, line);
                    if (line != "s")
                    {
                        qDebug() << "Error in layer format, missing 's' stroke separator";
                        return;
                    }

                    // Get line width
                    getline(in,line);
                    float lineWidth = stof(line);

                    Stroke* s = activeLayer->createNewStroke(lineWidth);

                    // Get number of control points
                    getline(in, line);
                    int numControlPoints = stoi(line);

                    // Add Control Points
                    for(int j = 0; j < numControlPoints; j++)
                    {

                        getline(in, line);
                        vector<string> coord = split(line, ' ');

                        float x = stof(coord[0]);
                        float y = stof(coord[1]);
                        float z = stof(coord[2]);

                        s->addPoint(QVector3D(x,y,z));

                    }
                    activeLayer->endCurrentStroke();

                }
            }
        }
    }
}

bool SketchManager::addLayer(string name)
{
    bool exists = false;

    for (int i = 0; i < layers.size(); i++)
    {
        exists = exists || layers[i]->getName() == name;
    }

    if (!exists)
    {
        activeLayer = new Layer(name);
        layers.push_back(activeLayer);
    }

    return !exists;
}

void SketchManager::deleteLayer(string name)
{

}

void SketchManager::changeActiveLayer(string name)
{

}

bool SketchManager::stroke(float mouseX, float mouseY, Camera *mCamera, float lineWidth, RayCaster rayCaster, QVector<QVector3D> geometry)
{
    QVector3D d = rayCaster.generateRayDirection(QVector2D(mouseX, mouseY), mCamera->getWidth(), mCamera->getHeight(), mCamera->getProjectionMatrix(), mCamera);
    QVector3D* intersect = rayCaster.findIntersection(mCamera->position(), d, geometry);


    if (intersect == NULL)
    {
        activeLayer->endCurrentStroke();
        return false;
    }
    else
    {
        activeLayer->createNewStroke(lineWidth);
        activeLayer->addPointToActiveStroke(*intersect);
        return true;
    }
}

void SketchManager::endStroke()
{
    activeLayer->endCurrentStroke();
}
