#include "ECManager.hpp"

// void ECManager::setSceneManager(std::shared_ptr<SceneManager> sceneManager) {
//     this->sceneManager = sceneManager;
//     this->activeScene = sceneManager->getActiveScene();
// }

void ECManager::tick()
{
    glm::vec3 camPos = m_camera->getCameraPosition();
    float pX = camPos.x;
    float pZ = camPos.z;

    for (auto& rc : renderComponents) {
        systemRender->Draw(rc);
    }

    for (auto& vc : vehicleComponents) {

        // Draw Vehicle
        systemVehicleRender->UpdateChassisModelMatrix(vc);
        systemVehicleRender->UpdateWheelModelMatrices(vc);
        systemVehicleRender->DrawVehicle(vc);

        // Give Game Input to Vehicle
        systemVehicleRender->updateVehicleControl(systemGameInput->currentAcceleration, systemGameInput->currentTurn, vc);

        // Update Map Chunking System (Physics) based on Vehicle Position
        for(auto& mcc : mapChunkComponents) {
            systemMapChunk->handleChunkUpdate(vc.worldPosition.x, vc.worldPosition.z, mcc);
        }
    }

}

ECManager::ECManager() {
    std::cout << "ECManager Constructor Called" << std::endl;
    systemRender = std::make_shared<RenderSystem>();
    systemVehicleRender = std::make_shared<VehicleRenderSystem>();
}


void ECManager::buildRenderComponent(std::string modelPath, std::string texPath, std::vector<int> meshIndices, float scale, bool cD, bool isTexAlpha) {
    RenderComponent rc = systemRender->buildRenderComponent(modelPath, texPath, meshIndices, scale, cD, isTexAlpha);  
    renderComponents.push_back(rc);
}

void ECManager::buildVehicleComponent(std::string vehicleChassisModel, std::string vcmTexPath, std::vector<int> vcmMeshIndices,
        std::string vehicleWheelModel, std::string vwmTexPath, std::vector<int> vwmMeshIndices,
        float scale, bool cD, bool isTexAlpha) {
    VehicleComponent vrc = systemVehicleRender->buildVehicleComponent(
        vehicleChassisModel, vcmTexPath, vcmMeshIndices, 
        vehicleWheelModel, vwmTexPath, vwmMeshIndices, scale, cD, isTexAlpha);
    vehicleComponents.push_back(vrc);
}

void ECManager::buildMapChunkComponent(const std::string& filename, float scaleFactor) {
    MapChunkComponent tcc = systemMapChunk->buildMapChunkComponent(filename, scaleFactor);
    mapChunkComponents.push_back(tcc);
}

void ECManager::setGameInput(std::shared_ptr<GameInput> gameInput) {
    systemGameInput = gameInput;
}



//* =============================== Getters/Setters to the "World" System ===============================


void ECManager::setCamera(std::shared_ptr<Camera> camera) {
    // Check if the input is valid
    if (camera) {
        m_camera = camera;

    } else {
        // Handle the error appropriately (e.g., logging, throwing an exception)
        Logger* logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid Camera Input !");
    }
}