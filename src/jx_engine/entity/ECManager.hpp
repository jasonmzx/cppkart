#ifndef ECMANAGER_HPP
#define ECMANAGER_HPP

#include <memory>
#include "jx_engine/core/SceneManager.hpp"

// Entity Components:
#include "jx_engine/component/EComponent.hpp"
#include "jx_engine/component/RenderComponent.hpp"
#include "jx_engine/component/PlayerVehicleComponent.hpp"

class ECManager { // Entity Component Manager
    public:
        //void update(float dt);
        void tick(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<GameInput> gameInput);

    private:
        std::shared_ptr<SceneManager> sceneManager;
        std::shared_ptr<Scene> activeScene;

};

#endif // ECMANAGER_HPP
