#ifndef __SCENE_HPP
#define __SCENE_HPP

#include <vector>
#include "My3DModel.hpp"

class Scene{
    public:
    Scene();
    ~Scene();
    bool addMy3DModel(My3DModel model);
    bool Scene::load3DModels();
    private:
        std::vector<My3DModel> models;
};

#endif