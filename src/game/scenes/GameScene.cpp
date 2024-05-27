#include "GameScene.hpp"

GameScene::GameScene(int WIN_W, int WIN_H) {
    
    camera = std::make_shared<Camera>(WIN_W, WIN_H, glm::vec3(0.0f, 10.0f, 2.0f));
    renderer = std::make_shared<GameGLRenderer>(WIN_W, WIN_H, camera.get());
    renderRsrcManager = std::make_shared<RenderRsrcManager>();

}

void GameScene::update(float dt) {
    // Update game logic
}

void GameScene::render() {
    // Render game objects
    renderer.get()->RenderPrep();
    ecManager.get()->tick(entities);

    camera.get()->Matrix(45.0f, 0.1f, 1000.0f, renderer.get()->mainShader, "camMatrix"); //! IMPORTANT
}

void GameScene::init() {

    std::shared_ptr<Entity> testEnt = std::make_shared<Entity>();

    auto testRenderComponent = std::make_shared<RenderComponent>("../../src/ressources/DE_Map1/Landscape01.obj",
                                                           "../../src/ressources/DE_Map1/Map01_Albedo.png", 
                                                           renderRsrcManager);

    testRenderComponent->SetGLContext(renderer.get()->useTextureLOC, renderer.get()->modelMatrixLOC, renderer.get()->colorUniformLocation);

    testEnt->addComponent(testRenderComponent);
    entities.push_back(testEnt);

    // Initialize game objects
    printf("GameScene initialized\n");

}

void GameScene::initECS(std::shared_ptr<SceneManager> sceneManager) {
    ecManager = std::make_unique<ECManager>();
}