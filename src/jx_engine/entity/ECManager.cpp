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
            
            // if (auto renderComponent = std::dynamic_pointer_cast<RenderComponent>(component)) {
            //     renderComponent.get()->Draw();
            // }

            if (auto playerVehicleComponent = std::dynamic_pointer_cast<PlayerVehicleComponent>(component)) {
                
                playerVehicleComponent.get()->updateVehicleControl(gameInput.get()->currentAcceleration, gameInput.get()->currentTurn);
                
                float pX = playerVehicleComponent.get()->vehiclePhysics.getX();
                float pY = playerVehicleComponent.get()->vehiclePhysics.getY();
                float pZ = playerVehicleComponent.get()->vehiclePhysics.getZ();

                dpX = pX;
                dpY = pY;
                dpZ = pZ;

                // printf("Player Vehicle Position: %f, %f, %f\n", pX, pY, pZ);

                terrainChunksComponents.get()->updateChunks(pX, pZ);
                
                if(followPlayerVehicle) {
                    camera.get()->VehicleFollowCamera(pX,pY,pZ);
                }
            }
        }
    }
}

void ECManager::renderPass(std::vector<std::shared_ptr<Entity>> entities) {
    for(auto entity : entities) {
        for (auto& component : entity.get()->components) {

            if (auto vehicleRenderComponent = std::dynamic_pointer_cast<VehicleRenderComponent>(component)) {
                btTransform vehicleTransform = playerVehicleComponent.get()->vehiclePhysics.GetTransform();
                btVector3 vehiclePosition = vehicleTransform.getOrigin();
                btQuaternion vehicleRotation = vehicleTransform.getRotation();

                glm::vec3 glmVehiclePosition(vehiclePosition.x(), vehiclePosition.y(), vehiclePosition.z());
                glm::quat glmVehicleRotation(vehicleRotation.w(), vehicleRotation.x(), vehicleRotation.y(), vehicleRotation.z());

                vehicleRenderComponent.get()->getTransforms(glmVehiclePosition, glmVehicleRotation);
                vehicleRenderComponent.get()->UpdateWheelTransforms(&playerVehicleComponent.get()->vehiclePhysics);
                vehicleRenderComponent.get()->DrawWheels();
            }

            // After getting the transforms, perform the rendering

            if (auto renderComponent = std::dynamic_pointer_cast<RenderComponent>(component)) {
                renderComponent.get()->Draw();
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

void ECManager::setPlayerVehicle(std::shared_ptr<PlayerVehicleComponent> playerVehicle) {
    // Check if the input is valid
    if (playerVehicle) {
        playerVehicleComponent = playerVehicle;
    } else {
        // Handle the error appropriately (e.g., logging, throwing an exception)
        Logger* logger = Logger::getInstance();
        logger->log(Logger::ERROR, "Invalid PlayerVehicleComponent input");
    }
}

void ECManager::resetPlayerVehicle() {
    playerVehicleComponent.get()->vehiclePhysics.ResetTransform();
}

std::string ECManager::debugStateSTR() {
    std::string debugState = "Player VPOS: " + std::to_string(dpX) + ", " + std::to_string(dpY) + ", " + std::to_string(dpZ);
    return debugState;
}