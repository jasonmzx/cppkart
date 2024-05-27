#include "ECManager.hpp"

// void ECManager::setSceneManager(std::shared_ptr<SceneManager> sceneManager) {
//     this->sceneManager = sceneManager;
//     this->activeScene = sceneManager.get()->getActiveScene();
// }

void ECManager::tick(std::vector<std::shared_ptr<Entity>> entities) {
    for(auto entity : entities) {

        for (auto& component : entity.get()->components) {
            
            if (auto renderComponent = std::dynamic_pointer_cast<RenderComponent>(component)) {
                renderComponent.get()->Draw();
            }
            
        }
    }
}



