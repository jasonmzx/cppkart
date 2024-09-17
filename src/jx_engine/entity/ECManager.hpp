#ifndef ECMANAGER_HPP
#define ECMANAGER_HPP

#include <memory>
#include <vector>
#include <unordered_map>

#include "jx_engine/core/SceneManager.hpp"

#include "Entity.hpp"

#include "jx_engine/render/GameGLRenderer.hpp"

// Entity Components:

#include "jx_engine/io/GameInput.hpp"

#include "jx_engine/render/gl/Camera.h"
#include "jx_engine/logs/Logger.hpp"

#include "jx_engine/event/Event.hpp"

// Better ECS:

#include "jx_engine/system/RenderSystem.hpp"
#include "jx_engine/system/VehicleRenderSystem.hpp"


class ECManager { // Entity Component Manager
    public:
        
        ECManager();

        using Callback = std::function<void(const Event&)>; // Constains a function that takes an Event as argument and returns void

        void subscribeToEvent(EventType type, Callback callback);
        void unsubscribeToEvent(EventType type, int id);
        void emitEvent(const Event& event);

        // Hash Map of Event Types and their uniquely identified Callbacks (ID, Callback)
        std::unordered_map<EventType, std::vector<std::pair<int, Callback>>> eventCallbacks;
        int nextId = 0;

        void setCamera(std::shared_ptr<Camera> camera);
        void setFreeCameraMode(bool freeCam);

        //* ------------------- Older Code ------------------- *//

        //void update(float dt);
        void tick();

        std::shared_ptr<RenderSystem> systemRender;
        std::vector<RenderComponent> renderComponents;

        std::shared_ptr<VehicleRenderSystem> systemVehicleRender;
        std::vector<VehicleComponent> vehicleComponents;

        void buildRenderComponent(std::string modelPath, std::string texPath, std::vector<int> meshIndices, float scale, bool cD, bool isTexAlpha);

        void buildVehicleComponent(std::string vehicleChassisModel, std::string vcmTexPath, std::vector<int> vcmMeshIndices,
        std::string vehicleWheelModel, std::string vwmTexPath, std::vector<int> vwmMeshIndices,
        float scale, bool cD, bool isTexAlpha);

    private:



        // old shit


        std::shared_ptr<SceneManager> sceneManager;
        std::shared_ptr<Scene> activeScene;
        std::shared_ptr<Camera> m_camera;

        float dpX; // Debug Player X
        float dpY; // Debug Player Y
        float dpZ; // Debug Player Z

        // Player Position:

        glm::vec3 playerPosition;

        int NRoadBarriers = 0;

};

#endif // ECMANAGER_HPP
