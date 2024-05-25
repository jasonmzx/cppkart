#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "jx_engine/core/Scene.hpp"

class GameScene : public Scene {
    public:
        void init() override;
        
        void update(float dt) override;
        void render() override;

};

#endif // SCENE_HPP
