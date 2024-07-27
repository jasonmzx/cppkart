#ifndef ECMANAGER_HPP
#define ECMANAGER_HPP

#include <memory>
#include <vector>
#include <unordered_map>

#include "jx_engine/core/SceneManager.hpp"

#include "Entity.hpp"

// Entity Components:
#include "jx_engine/component/EComponent.hpp"
#include "jx_engine/component/RenderComponent.hpp"
#include "jx_engine/component/PlayerVehicleComponent.hpp"
#include "jx_engine/component/TerrainChunksComponent.hpp"
#include "jx_engine/component/VehicleRenderComponent.hpp"
#include "jx_engine/component/MovableObjectComponent.hpp"

#include "jx_engine/render/gl/Camera.h"
#include "jx_engine/logs/Logger.hpp"

#include "jx_engine/event/Event.hpp"

class ECManager { // Entity Component Manager
    public:

        using Callback = std::function<void(const Event&)>; // Constains a function that takes an Event as argument and returns void

        void subscribeToEvent(EventType type, Callback callback);
        void unsubscribeToEvent(EventType type, int id);
        void emitEvent(const Event& event);

        // Hash Map of Event Types and their uniquely identified Callbacks (ID, Callback)
        std::unordered_map<EventType, std::vector<std::pair<int, Callback>>> eventCallbacks;
        int nextId = 0;

        void setCamera(std::shared_ptr<Camera> camera);
        void setFreeCameraMode(bool freeCam);

        void debugSetPlayerVehicleVelocity(float& velocity);

        //* ------------------- Older Code ------------------- *//

        //void update(float dt);
        void tick(std::vector<std::shared_ptr<Entity>>& entities, std::shared_ptr<GameInput> gameInput);
        
        void renderPass(std::vector<std::shared_ptr<Entity>>& entities);

        void setTerrainChunks(std::shared_ptr<TerrainChunksComponent> terrainChunks);
        void setPlayerVehicle(std::shared_ptr<PlayerVehicleComponent> playerVehicle);

        void resetPlayerVehicle();
        std::string debugStateSTR();


    private:
        std::shared_ptr<SceneManager> sceneManager;
        std::shared_ptr<Scene> activeScene;
        std::shared_ptr<Camera> m_camera;

        std::shared_ptr<TerrainChunksComponent> terrainChunksComponents;
        std::shared_ptr<PlayerVehicleComponent> playerVehicleComponent;

        // Debug State:

        float dpX; // Debug Player X
        float dpY; // Debug Player Y
        float dpZ; // Debug Player Z

        int NRoadBarriers = 0;

};

#endif // ECMANAGER_HPP
