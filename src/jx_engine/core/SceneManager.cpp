#include "SceneManager.hpp"

SceneManager::SceneManager() : activeScene(nullptr) {}

void SceneManager::render() {
    activeScene.get()->render();
}

std::shared_ptr<Scene> SceneManager::getActiveScene() {
    return activeScene;
}

void SceneManager::setActiveScene(std::shared_ptr<Scene> scene) {
    activeScene = scene;
}