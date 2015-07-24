#include "layerrenderer.h"

void renderLayer(Layer* layer, Camera* mCamera, GLLinesShader* shader)
{
    shader->bind();

    vector<Stroke*> strokes = layer->getStrokes();

    for (int i = 0; i < strokes.size(); i++)
    {
        QVector<QVector3D> verts = strokes[i]->getStroke();
        shader->setUniformValue("color", strokes[i]->getColor());
        shader->setUniformValue("thickness", strokes[i]->getLineWidth());
        shader->bindBuffers(verts);
        shader->render();
    }
}

void renderLayer(Layer* layer, Camera* mCamera, VertexBasedLineShader* shader)
{
    shader->bind();

    vector<Stroke*> strokes = layer->getStrokes();

    for (int i = 0; i < strokes.size(); i++)
    {
        QVector<QVector3D> verts = strokes[i]->getStroke();
        shader->setUniformValue("color", strokes[i]->getColor());
        shader->setUniformValue("thickness", strokes[i]->getLineWidth());
        shader->bindBuffers(verts);
        shader->render();
    }
}

void renderLayer(Layer* layer, Camera* mCamera, LineShader *shader)
{
    shader->bind();
    shader->setUniformValue("mMat", mCamera->getModelMatrix());
    shader->setUniformValue("vMat", mCamera->getViewMatrix());
    shader->setUniformValue("pMat", mCamera->getProjectionMatrix());

    vector<Stroke*> strokes = layer->getStrokes();

    for (int i = 0; i < strokes.size(); i++)
    {
        QVector<QVector3D> verts = strokes[i]->getStroke();
        shader->setUniformValue("color", strokes[i]->getColor());
        shader->setUniformValue("thickness", strokes[i]->getLineWidth());
        shader->bindBuffers(verts);
        shader->render();
    }
}

void renderLayer(Layer* layer, Camera* mCamera, ShaderProgram *shader)
{
    shader->bind();
    shader->setUniformValue("mMat", mCamera->getModelMatrix());
    shader->setUniformValue("vMat", mCamera->getViewMatrix());
    shader->setUniformValue("pMat", mCamera->getProjectionMatrix());

    vector<Stroke*> strokes = layer->getStrokes();

    for (int i = 0; i < strokes.size(); i++)
    {
        QVector<QVector3D> verts = strokes[i]->getStroke();
        shader->setUniformValue("color", strokes[i]->getColor());
        shader->setUniformValue("thickness", strokes[i]->getLineWidth());
        shader->bindBuffers(verts);
        shader->render();
    }
}
