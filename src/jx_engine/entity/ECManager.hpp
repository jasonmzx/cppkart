#ifndef ECMANAGER_HPP
#define ECMANAGER_HPP

#include <memory>
#include "jx_engine/core/SceneManager.hpp"

// Entity Components:
#include "jx_engine/component/EComponent.hpp"
#include "jx_engine/component/RenderComponent.hpp"

class ECManager { // Entity Component Manager
    public:
        //void update(float dt);
        void tick();

        void setSceneManager(std::shared_ptr<SceneManager> sceneManager);
    private:
        std::shared_ptr<SceneManager> sceneManager;
        std::shared_ptr<Scene> activeScene;

};

#endif // ECMANAGER_HPP
