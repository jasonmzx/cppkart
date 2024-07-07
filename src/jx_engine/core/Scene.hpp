#ifndef SCENE_HPP
#define SCENE_HPP

#include <SDL2/SDL.h>
#include <vector>  
#include <memory>

#include "jx_engine/entity/Entity.hpp"

class Scene {
    public:
        virtual void init() = 0;
        
        virtual void tickScene() = 0;

        virtual void updateImGui() = 0;

        virtual void updateScreenSize(int w, int h) = 0;

    public:
        std::vector<std::shared_ptr<Entity>> entities;

};

#endif // SCENE_HPP
