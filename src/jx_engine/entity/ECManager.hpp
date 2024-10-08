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
#include "jx_engine/system/MapChunkSystem.hpp"

#include "jx_engine/io/GameInput.hpp"

class ECManager { // Entity Component Manager
    public:
        
        ECManager();



        void setCamera(std::shared_ptr<Camera> camera);

        //* ------------------- Older Code ------------------- *//

        //void update(float dt);
        void tick();

        std::shared_ptr<RenderSystem> systemRender;
        std::vector<RenderComponent> renderComponents;

        std::shared_ptr<VehicleRenderSystem> systemVehicleRender;
        std::vector<VehicleComponent> vehicleComponents;

        std::shared_ptr<MapChunkSystem> systemMapChunk;
        std::vector<MapChunkComponent> mapChunkComponents;

        std::shared_ptr<GameInput> systemGameInput;

        void buildRenderComponent(std::string modelPath, std::string texPath, std::vector<int> meshIndices, float scale, bool cD, bool isTexAlpha);

        void buildVehicleComponent(std::string vehicleChassisModel, std::string vcmTexPath, std::vector<int> vcmMeshIndices,
        std::string vehicleWheelModel, std::string vwmTexPath, std::vector<int> vwmMeshIndices,
        float scale, bool cD, bool isTexAlpha);

        void buildMapChunkComponent(const std::string& filename, float scaleFactor);

        void setGameInput(std::shared_ptr<GameInput> gameInput);

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
