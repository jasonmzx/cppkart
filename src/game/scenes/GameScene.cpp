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

      btTransform protoPlaneTransform;
  protoPlaneTransform.setIdentity();
  protoPlaneTransform.setOrigin(btVector3(0, 0, 0));
  btStaticPlaneShape *plane = new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0));

  // Create Motion shape:
  btMotionState *motion = new btDefaultMotionState(protoPlaneTransform); //! He put btDefaultMotionShape

  btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
  info.m_friction = 2.0f;

  btRigidBody *planeBody = new btRigidBody(info);
  physicsWorld->dynamicsWorld->addRigidBody(planeBody);

    std::shared_ptr<Entity> testEnt = std::make_shared<Entity>();

    auto testRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_Map1/Landscape01.obj",
                                                           "../src/ressources/DE_Map1/Map01_Albedo.png", 
                                                           renderRsrcManager);

    testRenderComponent->SetGLContext(renderer.get()->useTextureLOC, renderer.get()->modelMatrixLOC, renderer.get()->colorUniformLocation);

    testEnt->addComponent(testRenderComponent);
    entities.push_back(testEnt);

    logger->log(Logger::INFO,"GameScene Loaded in " + std::to_string(entities.size()) + " entities");

}

void GameScene::initECS(std::shared_ptr<SceneManager> sceneManager) {
    ecManager = std::make_unique<ECManager>();
}