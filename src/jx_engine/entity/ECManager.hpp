#ifndef ECMANAGER_HPP
#define ECMANAGER_HPP

#include <memory>
#include "jx_engine/core/SceneManager.hpp"

// Entity Components:
#include "jx_engine/component/EComponent.hpp"
#include "jx_engine/component/RenderComponent.hpp"
#include "jx_engine/component/PlayerVehicleComponent.hpp"
#include "jx_engine/component/TerrainChunksComponent.hpp"
#include "jx_engine/component/VehicleRenderComponent.hpp"

#include "jx_engine/render/gl/Camera.h"

#include "jx_engine/logs/Logger.hpp"

class ECManager { // Entity Component Manager
    public:
        //void update(float dt);
        void tick(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<GameInput> gameInput, std::shared_ptr<Camera> camera, bool followPlayerVehicle);
        
        void renderPass(std::vector<std::shared_ptr<Entity>> entities);

        void setTerrainChunks(std::shared_ptr<TerrainChunksComponent> terrainChunks);
        void setPlayerVehicle(std::shared_ptr<PlayerVehicleComponent> playerVehicle);

        void resetPlayerVehicle();
        std::string debugStateSTR();


    private:
        std::shared_ptr<SceneManager> sceneManager;
        std::shared_ptr<Scene> activeScene;

        std::shared_ptr<TerrainChunksComponent> terrainChunksComponents;
        std::shared_ptr<PlayerVehicleComponent> playerVehicleComponent;

        // Debug State:

        float dpX; // Debug Player X
        float dpY; // Debug Player Y
        float dpZ; // Debug Player Z

};

#endif // ECMANAGER_HPP
