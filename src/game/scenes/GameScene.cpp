#include "GameScene.hpp"

GameScene::GameScene(int WIN_W, int WIN_H) {
    
    camera = std::make_unique<Camera>(WIN_W, WIN_H, glm::vec3(0.0f, 10.0f, 2.0f));
    renderer = std::make_shared<GameGLRenderer>(WIN_W, WIN_H, camera.get());

}

void GameScene::update(float dt) {
    // Update game logic
}

void GameScene::render() {
    // Render game objects
}

void GameScene::init() {

    std::shared_ptr<Entity> testEnt = std::make_shared<Entity>();
    testEnt.get()->addComponent(std::make_shared<RenderComponent>());

    entities.push_back(testEnt);

    // Initialize game objects
    printf("GameScene initialized\n");
}