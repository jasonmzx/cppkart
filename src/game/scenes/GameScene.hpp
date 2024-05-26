#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "jx_engine/core/Scene.hpp"
#include "jx_engine/component/RenderComponent.hpp"

#include "core/gl/Camera.h"

#include "jx_engine/gl/GameGLRenderer.hpp"

class GameScene : public Scene {
    public:
        GameScene(int WIN_W, int WIN_H);
        void init() override;
        
        void update(float dt) override;
        void render() override;
    
    private:
        std::unique_ptr<Camera> camera;
        std::shared_ptr<GameGLRenderer> renderer;

};

#endif // SCENE_HPP
