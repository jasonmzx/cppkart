#include "ECManager.hpp"

// void ECManager::setSceneManager(std::shared_ptr<SceneManager> sceneManager) {
//     this->sceneManager = sceneManager;
//     this->activeScene = sceneManager.get()->getActiveScene();
// }

void ECManager::tick(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<GameInput> gameInput,
    std::shared_ptr<Camera> camera, bool followPlayerVehicle
) {
    for(auto entity : entities) {

        for (auto& component : entity.get()->components) {
            
            if (auto renderComponent = std::dynamic_pointer_cast<RenderComponent>(component)) {
                renderComponent.get()->Draw();
            }

            if (auto playerVehicleComponent = std::dynamic_pointer_cast<PlayerVehicleComponent>(component)) {
                
                playerVehicleComponent.get()->updateVehicleControl(gameInput.get()->currentAcceleration, gameInput.get()->currentTurn);
                
                float pX = playerVehicleComponent.get()->vehiclePhysics.getX();
                float pY = playerVehicleComponent.get()->vehiclePhysics.getY();
                float pZ = playerVehicleComponent.get()->vehiclePhysics.getZ();

                terrainChunksComponents.get()->updateChunks(pX, pZ);
                
                if(followPlayerVehicle) {
                    camera.get()->VehicleFollowCamera(pX,pY,pZ);
                }
            }
        }
    }
}

void ECManager::setTerrainChunks(std::shared_ptr<TerrainChunksComponent> terrainChunks) {
    // Check if the input is valid
    if (terrainChunks) {
        terrainChunksComponents = terrainChunks;
    } else {
        // Handle the error appropriately (e.g., logging, throwing an exception)
        Logger* logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid TerrainChunksComponent input");
    }
}
