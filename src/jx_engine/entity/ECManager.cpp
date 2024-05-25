#include "ECManager.hpp"

void ECManager::setSceneManager(std::shared_ptr<SceneManager> sceneManager) {
    this->sceneManager = sceneManager;
    this->activeScene = sceneManager.get()->getActiveScene();
}

void ECManager::tick() {
    activeScene.get()->render();
}



