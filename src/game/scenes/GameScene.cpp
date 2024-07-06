#include "GameScene.hpp"

#include <chrono>
//#define BT_USE_DOUBLE_PRECISION 1

#define COLLISION_GROUP_CHUNKS 0x1
#define COLLISION_GROUP_ALL 0x2

Logger* GameScene::logger = Logger::getInstance();

GameScene* GameScene::instance = nullptr;

GameScene::GameScene(int WIN_W, int WIN_H, SDL_Window* window) {
    
    WIN_WIDTH = WIN_W;
    WIN_HEIGHT = WIN_H;

    this->SDL_window = window;

    instance = this;

    gContactAddedCallback = bulletCollisionCallback;

    // Rendering  
    camera = std::make_shared<Camera>(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 10.0f, 2.0f));
    renderer = std::make_shared<GameGLRenderer>(WIN_WIDTH, WIN_HEIGHT, camera.get());
    renderRsrcManager = std::make_shared<RenderRsrcManager>();

    // IO
    gameInput = std::make_shared<GameInput>();

    //* =================== Load Sound Effects =================== *//

    // Init SDL_mixer
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
      logger->log(Logger::ERROR, "SDL_mixer could not initialize! SDL_mixer Error: " + std::string(Mix_GetError()));
    }

    crashSound = Mix_LoadWAV("../src/ressources/sounds/zapsplat_car_crash_01.mp3");
    if (crashSound == nullptr) {
      logger->log(Logger::ERROR, "Failed to load sound effect! SDL_mixer Error: " + std::string(Mix_GetError()));
    }

}

bool GameScene::bulletCollisionCallback(btManifoldPoint& cp, 
  const btCollisionObjectWrapper* colObj0, int partId0, int index0, const btCollisionObjectWrapper* colObj1, int partId1, int index1) 
{
    //std::cout << "Collision Detected!" << std::endl;

    // Get the two colliding objects
    btCollisionObject* obj0 = (btCollisionObject*)colObj0->getCollisionObject();
    btCollisionObject* obj1 = (btCollisionObject*)colObj1->getCollisionObject();
    
      // Retrieve user pointers and cast them back to int
    int userPtr0 = reinterpret_cast<intptr_t>(obj0->getUserPointer());
    int userPtr1 = reinterpret_cast<intptr_t>(obj1->getUserPointer());

    // Retreive applied impulse
    float impulse = cp.getAppliedImpulse();

    if( impulse > 0.1f) {
    std::cout << "Collision Detected between objects with user pointers: "
              << userPtr0 << " and " << userPtr1 << " | IMPULSE: " << impulse << std::endl;
    }

    if( impulse > 5000.0f && GameScene::instance) {
        Mix_PlayChannel(-1, GameScene::instance->crashSound, 0); 
    }

    return false;
}

void GameScene::update(float dt) {
    // Update game logic
}

void GameScene::updateImGui() {
    
    // --- Speed Plot ----
    static float speed_values[90] = { 0 };
    static int speed_offset = 0; // Index of the current value

    // --- Inferencing Time Plot ----
    static float ECS_INFR_values[90] = { 0 };
    static int ECS_INFR_offset = 0; 

    //FPS Time Plot
    static float FPS_values[90] = { 0 };
    static int FPS_offset = 0; 

    static double refresh_time = 0.0;

    if (ImGui::Begin("Game Controls")) {
        if (ImGui::BeginTabBar("Tabs")) {
            
            if (ImGui::BeginTabItem("DEBUG TOOLS")) { // Debug Tools Tab
                

                ImGui::Text("Vehicle Speed: %.2f", vSpeed);
                ImGui::Spacing();

                if(ImGui::GetTime() > refresh_time) {
                    speed_values[speed_offset] = vSpeed;
                    speed_offset = (speed_offset + 1) % IM_ARRAYSIZE(speed_values); // offset modulo array size, to wrap around
                }

                ImGui::PlotLines("", speed_values, IM_ARRAYSIZE(speed_values), speed_offset, NULL, 0.0f, 100.0f, ImVec2(0, 80));

                ImGui::Spacing();

                if (ImGui::Button("Reset Vehicle")) {
                    ecManager.get()->resetPlayerVehicle();
                }

                if (ImGui::Button("Toggle Bullet Debug Draw")) {
                    isBulletDebugDraw = !isBulletDebugDraw;
                }

                ImGui::Text("%s", ecManager.get()->debugStateSTR().c_str());

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

    // btCollisionWorld::ClosestRayResultCallback rayCallback(btVector3(camPos.x, camPos.y, camPos.z), btVector3(camLookAt.x, camLookAt.y, camLookAt.z));
    // physicsWorld->dynamicsWorld->rayTest(btVector3(camPos.x, camPos.y, camPos.z), btVector3(camLookAt.x, camLookAt.y, camLookAt.z), rayCallback);


    auto start = std::chrono::high_resolution_clock::now();

    vSpeed = 0.0f;

    ecManager->tick(entities, gameInput, camera, followPlayerVehicle, vSpeed); // ECS System Tick
    ecManager->renderPass(entities); // ECS Render Pass

    auto end = std::chrono::high_resolution_clock::now();
    ecInferenceTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    
    //Get and Draw Camera Ray Cast (todo make this optional)

    glm::vec3 rayStart, rayEnd;
    camera.get()->GenerateRay(rayStart, rayEnd, 1000.0f);

    if(isBulletDebugDraw) {
      renderer.get()->DebugDrawLine(rayStart, rayEnd, glm::vec3(1.0f, 0.0f, 0.0f));
      renderer.get()->DebugRender();
    }

    // Bullet Raycasting: 
    btCollisionWorld::ClosestRayResultCallback rayCallback(btVector3(rayStart.x, rayStart.y, rayStart.z), btVector3(rayEnd.x, rayEnd.y, rayEnd.z));
    
    rayCallback.m_collisionFilterGroup = COLLISION_GROUP_ALL | COLLISION_GROUP_CHUNKS;
    
    physicsWorld->dynamicsWorld->rayTest(btVector3(rayStart.x, rayStart.y, rayStart.z), btVector3(rayEnd.x, rayEnd.y, rayEnd.z), rayCallback);

    if(rayCallback.hasHit()) {
        std::cout << "Ray hit at: " << rayCallback.m_hitPointWorld.getX() << " " << rayCallback.m_hitPointWorld.getY() << " " << rayCallback.m_hitPointWorld.getZ() << std::endl;
    }



    camera.get()->Matrix(45.0f, 0.1f, 9000.0f, renderer.get()->mainShader, "camMatrix"); //! IMPORTANT

    //TODO: Accumulate deltaTime and pass it to physicsWorld->dynamicsWorld->stepSimulation
    //world.get()->physicsWorld->dynamicsWorld->stepSimulation(deltaTimeWithTimeScale, 2, deltaTime);
    physicsWorld->dynamicsWorld->stepSimulation(1.0f / 60.0f);
}

void GameScene::procGameInputs() {

  const Uint8 *KB_InputState = SDL_GetKeyboardState(NULL);
  
  GameInput* gameInputPtr = gameInput.get();
  gameInputPtr->keyboardUpdateInput(KB_InputState);

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

    // Joystick input handling
  if (gGameController != nullptr) {
        SDL_JoystickUpdate(); // Update joystick state

        // Left joystick
        int leftX = SDL_JoystickGetAxis(gGameController, 0); // X axis
        int leftY = SDL_JoystickGetAxis(gGameController, 1); // Y axis

        // LT and RT triggers
        int lt = SDL_JoystickGetAxis(gGameController, 2); // LT axis (Left Trigger)
        int rt = SDL_JoystickGetAxis(gGameController, 5); // RT axis (Right Trigger)

        gameInputPtr->xboxControllerUpdateInput(leftX, leftY, lt, rt);

        // Right joystick
        int rightX = SDL_JoystickGetAxis(gGameController, 3); // X axis
        int rightY = SDL_JoystickGetAxis(gGameController, 4); // Y axis

        // printf("Left Joystick - X: %d, Y: %d\n", leftX, leftY);
        // printf("Right Joystick - X: %d, Y: %d\n", rightX, rightY);
        // printf("Left Trigger (LT): %d\n", lt);
        // printf("Right Trigger (RT): %d\n", rt);

        // Print button states
        for (int i = 0; i < SDL_JoystickNumButtons(gGameController); i++) {
            if (SDL_JoystickGetButton(gGameController, i) == SDL_PRESSED) {
                printf("Button %d pressed\n", i);
            }
        }
  }

}


void GameScene::load_HighRoadHills_Map(std::shared_ptr<Entity> terrainEntity) {
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

    auto terrainChunks_physics_Component = std::make_shared<TerrainChunksComponent>("../src/ressources/DE_MAP0/chunks", terrainEntityScale);
    ecManager.get()->setTerrainChunks(terrainChunks_physics_Component);

    // Add Render Components:
    terrainEntity->addComponent(terrainRenderComponent);
    terrainEntity->addComponent(terrainRoadRenderComponent);
    terrainEntity->addComponent(terrainBottomRoadRenderComponent);
    
    // Add Physics Component:
    terrainEntity->addComponent(terrainChunks_physics_Component);
}


void GameScene::init() {

    // * =================== Joystick Input =================== * //

  const int JOYSTICK_DEAD_ZONE = 4000;

  const uint8_t* state = SDL_GetKeyboardState(nullptr);
  gGameController = nullptr;


        //Check for joysticks
  if( SDL_NumJoysticks() < 1 )
  {
    printf( "Warning: No joysticks connected!\n" );
  }
  else
  {
            //Load joystick
            gGameController = SDL_JoystickOpen( 0 );
            if( gGameController == nullptr )
            {
                printf( "Warning: Unable to open game controller! SDL Error: %s\n", SDL_GetError() );
            }

            //Load second joystick on xbox 360 controller

            printf(
              "JOYSTICK CONNECTION :-)\n"
            );
  }    

    // Init reference to physics singleton


    physicsWorld = PhysicsWorldSingleton::getInstance();

  //!__ Prototype Plane

  btTransform protoPlaneTransform;
  protoPlaneTransform.setIdentity();
  protoPlaneTransform.setOrigin(btVector3(0, 0, 0));
  btStaticPlaneShape *plane = new btStaticPlaneShape(btVector3(0, 1, 0), btScalar(0));

  // Create Motion shape:
  btMotionState *motion = new btDefaultMotionState(protoPlaneTransform); //! He put btDefaultMotionShape

  btRigidBody::btRigidBodyConstructionInfo info(0.0, motion, plane);
  info.m_friction = 2.0f;

  btRigidBody *planeBody = new btRigidBody(info);

  //body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
  planeBody->setCollisionFlags(planeBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

  physicsWorld->dynamicsWorld->addRigidBody(planeBody, COLLISION_GROUP_CHUNKS, COLLISION_GROUP_ALL);
  
  //!__ ENDOF Prototype Plane 

    // auto terrainRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_Map1/Landscape01.obj",
    //                                                        "../src/ressources/DE_Map1/Map01_Albedo.png", 
    //                                                        renderRsrcManager);

    //* ----------------- Terrain Entity Definition ----------------- *//

    std::shared_ptr<Entity> terrainEntity = std::make_shared<Entity>();
    load_HighRoadHills_Map(terrainEntity); 
    entities.push_back(terrainEntity);

    //* ----------------- Vehicle Entity Definition ----------------- *//

    std::shared_ptr<Entity> playerVehicleEntity = std::make_shared<Entity>();

    //auto playerVehicleComponent = std::make_shared<PlayerVehicleComponent>(10.0f, 10.0f, 10.0f);
    auto playerVehicleComponent = std::make_shared<PlayerVehicleComponent>(135.0f, 135.0f, -165.0f);
    
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