#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "jx_engine/core/SceneManager.hpp"
#include "jx_engine/core/Scene.hpp"
#include "jx_engine/component/RenderComponent.hpp"
#include "jx_engine/entity/ECManager.hpp"
#include "jx_engine/gl/GameGLRenderer.hpp"


//! Importants below need to be changed eventually
#include "core/gl/Camera.h"
#include "engine/rendering/RenderRsrcManager.hpp"


class GameScene : public Scene {
    public:
        GameScene(int WIN_W, int WIN_H);
        void init() override;
        void initECS(std::shared_ptr<SceneManager> sceneManager);
        
        void update(float dt) override;
        void render() override;

        std::unique_ptr<ECManager> ec;

        std::shared_ptr<Camera> camera;
    
    private:

        std::unique_ptr<ECManager> ecManager;


        std::shared_ptr<GameGLRenderer> renderer;
        std::shared_ptr<RenderRsrcManager> renderRsrcManager;

};

#endif // SCENE_HPP
