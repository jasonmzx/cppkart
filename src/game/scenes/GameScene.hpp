#ifndef GAMESCENE_HPP
#define GAMESCENE_HPP

#include "jx_engine/core/SceneManager.hpp"
#include "jx_engine/core/Scene.hpp"
#include "jx_engine/component/RenderComponent.hpp"
#include "jx_engine/entity/ECManager.hpp"

#include "jx_engine/render/GameGLRenderer.hpp"
#include "jx_engine/render/RenderRsrcManager.hpp"
#include "jx_engine/render/gl/Camera.h"

#include "jx_engine/physics/PhysicsWorldSingleton.h"
#include "jx_engine/physics/BulletDebugDrawer.cpp"

class GameScene : public Scene {
    public:
        GameScene(int WIN_W, int WIN_H);
        void init() override;
        void initECS(std::shared_ptr<SceneManager> sceneManager);
        
        void update(float dt) override;
        void render() override;

        std::unique_ptr<ECManager> ec;
        std::shared_ptr<Camera> camera;
        
        PhysicsWorldSingleton *physicsWorld;

    private:

        std::unique_ptr<ECManager> ecManager;

        std::shared_ptr<GameGLRenderer> renderer;
        std::shared_ptr<RenderRsrcManager> renderRsrcManager;

        static Logger* logger;
};

#endif // SCENE_HPP
