#include "ECManager.hpp"

// void ECManager::setSceneManager(std::shared_ptr<SceneManager> sceneManager) {
//     this->sceneManager = sceneManager;
//     this->activeScene = sceneManager.get()->getActiveScene();
// }

void ECManager::tick(std::vector<std::shared_ptr<Entity>> entities, std::shared_ptr<GameInput> gameInput) {
    for(auto entity : entities) {

        for (auto& component : entity.get()->components) {
            
            if (auto renderComponent = std::dynamic_pointer_cast<RenderComponent>(component)) {
                renderComponent.get()->Draw();
            }

            if (auto playerVehicleComponent = std::dynamic_pointer_cast<PlayerVehicleComponent>(component)) {
                playerVehicleComponent.get()->updateVehicleControl(gameInput.get()->currentAcceleration, gameInput.get()->currentTurn);
            }
            
        }
    }
}



