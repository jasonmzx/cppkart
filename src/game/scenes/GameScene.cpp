#include "GameScene.hpp"

Logger* GameScene::logger = Logger::getInstance();

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
    renderer.get()->DebugRender();

    camera.get()->Matrix(45.0f, 0.1f, 1000.0f, renderer.get()->mainShader, "camMatrix"); //! IMPORTANT

    //world.get()->physicsWorld->dynamicsWorld->stepSimulation(deltaTimeWithTimeScale, 2, deltaTime);
    physicsWorld->dynamicsWorld->stepSimulation(1.0f / 60.0f);
}

void GameScene::init() {
    
    // Init reference to physics singleton

    physicsWorld = PhysicsWorldSingleton::getInstance();

    std::shared_ptr<Entity> terrainEntity = std::make_shared<Entity>();

    // auto terrainRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_Map1/Landscape01.obj",
    //                                                        "../src/ressources/DE_Map1/Map01_Albedo.png", 
    //                                                        renderRsrcManager);

    auto terrainRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_Aztec/DE_AZTEC.obj",
                                                           "../src/ressources/DE_Map1/Map01_Albedo.png", 
                                                           renderRsrcManager);

    terrainRenderComponent->SetGLContext(renderer.get()->useTextureLOC, renderer.get()->modelMatrixLOC, renderer.get()->colorUniformLocation);

    terrainEntity->addComponent(terrainRenderComponent);

    entities.push_back(terrainEntity);

    std::vector<GLfloat> verts = terrainRenderComponent.get()->Geom.get()->_vertices;

    physicsChunkManager = std::make_unique<PhysicsChunkManager>(verts, "../src/ressources/chunk_map.txt");
    
    physicsChunkManager.get()->ActiveAll();

    logger->log(Logger::INFO,"GameScene Loaded in " + std::to_string(entities.size()) + " entities");



    // Init ECS



}

void GameScene::initECS(std::shared_ptr<SceneManager> sceneManager) {
    ecManager = std::make_unique<ECManager>();
}