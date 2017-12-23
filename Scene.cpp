#include "Scene.hpp"


Scene::Scene()
{

}

bool Scene::addMy3DModel(My3DModel model)
{
    bool statusOK = true;
    this->models.push_back(model);
    return statusOK;
}