#include "GameScene.hpp"

#include <chrono>

Logger* GameScene::logger = Logger::getInstance();

GameScene::GameScene(int WIN_W, int WIN_H, SDL_Window* window) {
    
    WIN_WIDTH = WIN_W;
    WIN_HEIGHT = WIN_H;

    this->SDL_window = window;

    // Rendering
    camera = std::make_shared<Camera>(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 10.0f, 2.0f));
    renderer = std::make_shared<GameGLRenderer>(WIN_WIDTH, WIN_HEIGHT, camera.get());
    renderRsrcManager = std::make_shared<RenderRsrcManager>();

    // IO
    gameInput = std::make_shared<GameInput>();

}

void GameScene::update(float dt) {
    // Update game logic
}

void GameScene::updateImGui() {
            // Example data for plotting
        static float values[90] = { 0 };
        static int values_offset = 0;
        static double refresh_time = 0.0;

        // Create a simple line plot
        if (ImGui::GetTime() > refresh_time)
        {
            values[values_offset] = ecInferenceTimeMS;
            values_offset = (values_offset + 1) % IM_ARRAYSIZE(values);
            refresh_time = ImGui::GetTime() + 1.0f / 60.0f;
        }
        ImGui::Begin("FPS Counter");
        ImGui::PlotLines("Frame Times", values, IM_ARRAYSIZE(values), values_offset, NULL, 0.0f, 100.0f, ImVec2(0, 80));
        ImGui::End();
}


void GameScene::render() {

    procGameInputs();

    // Render game objects

    renderer.get()->RenderPrep();

    auto start = std::chrono::high_resolution_clock::now();

    ecManager->tick(entities, gameInput, camera, followPlayerVehicle); // ECS System Tick

    auto end = std::chrono::high_resolution_clock::now();
    ecInferenceTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    printf("ECS Inference Time: %f\n", ecInferenceTimeMS);

    renderer.get()->DebugRender();

    camera.get()->Matrix(45.0f, 0.1f, 9000.0f, renderer.get()->mainShader, "camMatrix"); //! IMPORTANT

    //TODO: Accumulate deltaTime and pass it to physicsWorld->dynamicsWorld->stepSimulation
    //world.get()->physicsWorld->dynamicsWorld->stepSimulation(deltaTimeWithTimeScale, 2, deltaTime);
    physicsWorld->dynamicsWorld->stepSimulation(1.0f / 60.0f);
}

void GameScene::procGameInputs() {

  const Uint8 *KB_InputState = SDL_GetKeyboardState(NULL);
  gameInput.get()->keyboardUpdateInput(KB_InputState);

  // Some Direct Key Actions
  if(KB_InputState[SDL_SCANCODE_ESCAPE]) { // Free the mouse
    trackMouse = !trackMouse;
  }

  if(KB_InputState[SDL_SCANCODE_F1]) { // Toggle Vehicle Camera
    followPlayerVehicle = !followPlayerVehicle;
  }

  if (trackMouse)
  { // Let the Mouse handle the camera or not

    // Handles mouse inputs
    int mouseX, mouseY;
    Uint32 mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

    if (firstClick)
    {
      // Center the mouse cursor
      SDL_WarpMouseInWindow(SDL_window, WIN_WIDTH / 2, WIN_HEIGHT / 2);
      firstClick = false;
      SDL_ShowCursor(SDL_DISABLE); // Hide the mouse cursor
    }
    else
    {
      // SDL doesn't directly give us the previous position of the cursor,
      // so we compute the motion based on the current position and centering the cursor.
      int mouzDeltaX = mouseX - WIN_WIDTH / 2;
      int mouzDeltaY = mouseY - WIN_HEIGHT / 2;

        camera->Inputs();
        camera->ProcessMouseLook(mouzDeltaX, mouzDeltaY);

        SDL_WarpMouseInWindow(SDL_window, WIN_WIDTH / 2, WIN_HEIGHT / 2);
    }
  }

}

void GameScene::init() {
    
    // Init reference to physics singleton

    physicsWorld = PhysicsWorldSingleton::getInstance();

    //!__

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
  

    //!__

    std::shared_ptr<Entity> terrainEntity = std::make_shared<Entity>();

    // auto terrainRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_Map1/Landscape01.obj",
    //                                                        "../src/ressources/DE_Map1/Map01_Albedo.png", 
    //                                                        renderRsrcManager);

    //* ----------------- Terrain Entity Definition ----------------- *//

    auto terrainRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_Aztec/DE_AZTEC.obj",
                                                           "../src/ressources/DE_Map1/Map01_Albedo.png", 
                                                           renderRsrcManager);

    terrainRenderComponent->SetGLContext(renderer.get()->useTextureLOC, renderer.get()->modelMatrixLOC, renderer.get()->colorUniformLocation);

    auto terrainChunks_physics_Component = std::make_shared<TerrainChunksComponent>("../src/ressources/full_chunk_map.txt");
    ecManager.get()->setTerrainChunks(terrainChunks_physics_Component);

    terrainEntity->addComponent(terrainRenderComponent);
    terrainEntity->addComponent(terrainChunks_physics_Component);

    entities.push_back(terrainEntity);

    //* ----------------- Vehicle Entity Definition ----------------- *//

    std::shared_ptr<Entity> playerVehicleEntity = std::make_shared<Entity>();

    auto playerVehicleComponent = std::make_shared<PlayerVehicleComponent>();

    playerVehicleEntity->addComponent(playerVehicleComponent);
    
    entities.push_back(playerVehicleEntity);

    logger->log(Logger::INFO,"GameScene Loaded in " + std::to_string(entities.size()) + " entities");

}

void GameScene::initECS(std::shared_ptr<SceneManager> sceneManager) {
    ecManager = std::make_unique<ECManager>();
}