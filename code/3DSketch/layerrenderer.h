#ifndef LAYERRENDERER_H
#define LAYERRENDERER_H

#include "layer.h"
#include "gllinesshader.h"
#include "camera.h"
#include "vertexbasedlineshader.h"
#include "lineshader.h"


void renderLayer(Layer* layer, Camera* mCamera, GLLinesShader *shader);
void renderLayer(Layer* layer, Camera* mCamera, VertexBasedLineShader *shader);
void renderLayer(Layer* layer, Camera* mCamera, LineShader *shader);
void renderLayer(Layer* layer, Camera* mCamera, ShaderProgram *shader);

#endif // LAYERRENDERER_H
