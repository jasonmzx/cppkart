#include "SceneManager.hpp"

SceneManager::SceneManager() : activeScene(nullptr) {}

std::shared_ptr<Scene> SceneManager::getActiveScene() {
    return activeScene;
}

void SceneManager::setActiveScene(std::shared_ptr<Scene> scene) {
    activeScene = scene;
}