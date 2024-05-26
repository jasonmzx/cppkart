#include "ECManager.hpp"

void ECManager::setSceneManager(std::shared_ptr<SceneManager> sceneManager) {
    this->sceneManager = sceneManager;
    this->activeScene = sceneManager.get()->getActiveScene();
}

void ECManager::tick() {
    activeScene.get()->render();

    for (auto entity : activeScene.get()->entities) {

        for (auto& component : entity.get()->components) {
            
            if (auto renderComponent = std::dynamic_pointer_cast<RenderComponent>(component)) {
                //renderComponent->render();
            }
            
        }
    }
}



