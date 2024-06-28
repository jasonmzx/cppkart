#include "GameScene.hpp"

#include <chrono>
//#define BT_USE_DOUBLE_PRECISION 1

Logger* GameScene::logger = Logger::getInstance();

GameScene::GameScene(int WIN_W, int WIN_H, SDL_Window* window) {
    
    WIN_WIDTH = WIN_W;
    WIN_HEIGHT = WIN_H;

    this->SDL_window = window;

    gContactAddedCallback = bulletCollisionCallback;

    // Rendering  
    camera = std::make_shared<Camera>(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 10.0f, 2.0f));
    renderer = std::make_shared<GameGLRenderer>(WIN_WIDTH, WIN_HEIGHT, camera.get());
    renderRsrcManager = std::make_shared<RenderRsrcManager>();

    // IO
    gameInput = std::make_shared<GameInput>();

}

bool GameScene::bulletCollisionCallback(btManifoldPoint& cp, 
  const btCollisionObjectWrapper* colObj0, int partId0, int index0, const btCollisionObjectWrapper* colObj1, int partId1, int index1) {
    
    //std::cout << "Collision Detected!" << std::endl;

    // Get the two colliding objects
    btCollisionObject* obj0 = (btCollisionObject*)colObj0->getCollisionObject();
    btCollisionObject* obj1 = (btCollisionObject*)colObj1->getCollisionObject();
    
      // Retrieve user pointers and cast them back to int
    int userPtr0 = reinterpret_cast<intptr_t>(obj0->getUserPointer());
    int userPtr1 = reinterpret_cast<intptr_t>(obj1->getUserPointer());

    // Retreive applied impulse
    float impulse = cp.getAppliedImpulse();

    std::cout << "Collision Detected between objects with user pointers: "
              << userPtr0 << " and " << userPtr1 << " | IMPULSE: " << impulse << std::endl;
    
    
    return false;
}


void GameScene::update(float dt) {
    // Update game logic
}

void GameScene::updateImGui() {
    
    // Inferencing Time Plot
    static float ECS_INFR_values[90] = { 0 };
    static int ECS_INFR_offset = 0; // Index of the current value

    //FPS Time Plot
    static float FPS_values[90] = { 0 };
    static int FPS_offset = 0; // Index of the current value

    static double refresh_time = 0.0;

    if (ImGui::Begin("Game Controls")) {
        if (ImGui::BeginTabBar("Tabs")) {
            
            if (ImGui::BeginTabItem("DEBUG TOOLS")) { // Debug Tools Tab
                
                if (ImGui::Button("Reset Vehicle")) {
                    ecManager.get()->resetPlayerVehicle();
                }

                if (ImGui::Button("Toggle Bullet Debug Draw")) {
                    isBulletDebugDraw = !isBulletDebugDraw;
                }

                ImGui::Text(ecManager.get()->debugStateSTR().c_str());

                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("PERFORMANCE")) { // Performance Tab
                
                // Create a simple line plot
                if (ImGui::GetTime() > refresh_time) {
                    ECS_INFR_values[ECS_INFR_offset] = ecInferenceTimeMS;
                    ECS_INFR_offset = (ECS_INFR_offset + 1) % IM_ARRAYSIZE(ECS_INFR_values); // offset modulo array size, to wrap around

                    FPS_values[FPS_offset] = ImGui::GetIO().Framerate;
                    FPS_offset = (FPS_offset + 1) % IM_ARRAYSIZE(FPS_values); // offset modulo array size, to wrap around
                    refresh_time = ImGui::GetTime() + 1.0f / 60.0f;
                }



                float framerate = ImGui::GetIO().Framerate;

                char formatted_inference_STR[50];
                char formatted_fps_STR[50];

                sprintf(formatted_fps_STR, "FPS: %.1f", framerate);
                sprintf(formatted_inference_STR, "ECS (ms): %.2f", ecInferenceTimeMS);

                ImGui::PlotLines(formatted_inference_STR, ECS_INFR_values, IM_ARRAYSIZE(ECS_INFR_values), ECS_INFR_offset, NULL, 0.0f, 100.0f, ImVec2(0, 80));
                ImGui::PlotLines(formatted_fps_STR, FPS_values, IM_ARRAYSIZE(FPS_values), FPS_offset, NULL, 0.0f, 100.0f, ImVec2(0, 80));

                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
        ImGui::End();
    }
}



void GameScene::render() {

    procGameInputs();

    // Render game objects

    renderer.get()->RenderPrep();

    auto start = std::chrono::high_resolution_clock::now();

    ecManager->tick(entities, gameInput, camera, followPlayerVehicle); // ECS System Tick
    ecManager->renderPass(entities); // ECS Render Pass

    auto end = std::chrono::high_resolution_clock::now();
    ecInferenceTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if(isBulletDebugDraw) {
      renderer.get()->DebugRender();
    }

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
  
  //! ___

// // Initialize triangle mesh shape
// btTriangleMesh* triangleMesh = new btTriangleMesh();
// triangleMesh->addTriangle(btVector3(0, 0, 0),
//                           btVector3(-14.318789, 21.87693, -91.302094),
//                           btVector3(-14.203687, 22.688904, -91.920738));

// btBvhTriangleMeshShape* meshShape = new btBvhTriangleMeshShape(triangleMesh, true);
// btTransform meshTransform;
// meshTransform.setIdentity();
// btDefaultMotionState* meshMotionState = new btDefaultMotionState(meshTransform);
// btRigidBody::btRigidBodyConstructionInfo meshRigidBodyCI(0, meshMotionState, meshShape, btVector3(0, 0, 0));
// btRigidBody* meshRigidBody = new btRigidBody(meshRigidBodyCI);

// // Check if the meshRigidBody is valid
// if (!meshRigidBody) {
//     std::cerr << "Failed to initialize meshRigidBody!" << std::endl;
//     return;
// }

// physicsWorld->dynamicsWorld->addRigidBody(meshRigidBody);
//     //!__

    std::shared_ptr<Entity> terrainEntity = std::make_shared<Entity>();

    // auto terrainRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_Map1/Landscape01.obj",
    //                                                        "../src/ressources/DE_Map1/Map01_Albedo.png", 
    //                                                        renderRsrcManager);

    //* ----------------- Terrain Entity Definition ----------------- *//

    float terrainEntityScale = 15.0f;


    auto terrainRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_MAP0/MAPOI.obj",
                                                           "../src/ressources/DE_Map1/Map01_Albedo.png", 
                                                           renderRsrcManager, 0, true, false);

    terrainRenderComponent->SetGLContext(renderer.get()->useTextureLOC, renderer.get()->modelMatrixLOC, renderer.get()->colorUniformLocation, terrainEntityScale);

    auto terrainRoadRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_MAP0/MAPOI.obj",
                                                           "../src/ressources/DE_MAP0/BIG_ROAD_TEX.jpg", 
                                                           renderRsrcManager, 2, true, false);

    terrainRoadRenderComponent->SetGLContext(renderer.get()->useTextureLOC, renderer.get()->modelMatrixLOC, renderer.get()->colorUniformLocation, terrainEntityScale);

    auto terrainBottomRoadRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_MAP0/MAPOI.obj",
                                                           "../src/ressources/DE_MAP0/STONE_WALL_04.jpg", 
                                                           renderRsrcManager, 3, false, false);

    terrainBottomRoadRenderComponent->SetGLContext(renderer.get()->useTextureLOC, renderer.get()->modelMatrixLOC, renderer.get()->colorUniformLocation, terrainEntityScale);                                                          


    auto terrainChunks_physics_Component = std::make_shared<TerrainChunksComponent>("../src/ressources/DE_MAP0/Small_Chunks_MAPOI.txt", terrainEntityScale);
    ecManager.get()->setTerrainChunks(terrainChunks_physics_Component);

    terrainEntity->addComponent(terrainRenderComponent);
    terrainEntity->addComponent(terrainRoadRenderComponent);
    terrainEntity->addComponent(terrainBottomRoadRenderComponent);

    terrainEntity->addComponent(terrainChunks_physics_Component);

    entities.push_back(terrainEntity);

    //* ----------------- Vehicle Entity Definition ----------------- *//

    std::shared_ptr<Entity> playerVehicleEntity = std::make_shared<Entity>();

    auto playerVehicleComponent = std::make_shared<PlayerVehicleComponent>();
    ecManager.get()->setPlayerVehicle(playerVehicleComponent);

    playerVehicleEntity->addComponent(playerVehicleComponent);
    

    auto playerVehicleRenderComponent = std::make_shared<VehicleRenderComponent>("../src/ressources/volga/volga.obj", "../src/ressources/first_car_wheel.obj",
                                                           "../src/ressources/volga/volga.png", 
                                                           renderRsrcManager, 0, true);

    playerVehicleRenderComponent->SetGLContext(renderer.get()->useTextureLOC, renderer.get()->modelMatrixLOC, renderer.get()->colorUniformLocation, 1.0f);

    playerVehicleEntity->addComponent(playerVehicleRenderComponent);                                                           

    entities.push_back(playerVehicleEntity);

    logger->log(Logger::INFO,"GameScene Loaded in " + std::to_string(entities.size()) + " entities");
}

void GameScene::initECS(std::shared_ptr<SceneManager> sceneManager) {
    ecManager = std::make_unique<ECManager>();
}