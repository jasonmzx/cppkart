#include "GameScene.hpp"

#include <chrono>
//#define BT_USE_DOUBLE_PRECISION 1

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
    renderer = GameGLRenderer::getInstance(WIN_WIDTH, WIN_HEIGHT, camera.get()); // Init Renderer

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
                    ecManager->resetPlayerVehicle();
                }

                if (ImGui::Button("Toggle Bullet Debug Draw")) {
                    isBulletDebugDraw = !isBulletDebugDraw;
                    ecManager->toggleAICylinders();
                }

                if (ImGui::Button("Activate AI Control")) {
                    ecManager->toggleAIVehicleControl();
                }

                if (ImGui::Button("Spawn Barrier")) {
                    glm::vec3 player_pos = ecManager->getLastPlayerPos();

                    player_pos.y += 10.0f;

                    makeBarrier(player_pos.x, player_pos.y, player_pos.z);
                }



                 if (ImGui::Button("RENDER :: View GGX Normals")) {
                    ecManager->toggleNormalsShader();
                  }

                // This is the X,Y,Z Location of the vehicle
                ImGui::Text("%s", ecManager->debugStateSTR().c_str());
   
                // Input fields for x, y, z coordinates
                static float cam_tp_x = 0.0f, cam_tp_y = 0.0f, cam_tp_z = 0.0f;
                ImGui::InputFloat("X", &cam_tp_x);
                ImGui::InputFloat("Y", &cam_tp_y);
                ImGui::InputFloat("Z", &cam_tp_z);

                if (ImGui::Button("Teleport Camera")) {
                    printf("Teleporting Camera to: %f, %f, %f\n", cam_tp_x, cam_tp_y, cam_tp_z);

                    glm::vec3 teleport_to = glm::vec3(cam_tp_x, cam_tp_y, cam_tp_z);
                    camera->setCameraPosition(teleport_to);
                } 

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

void GameScene::tickScene() { //* Main Game Loop Function (Scene Tick)
    update(1.0f / 60.0f);
    render();
}

void GameScene::update(float dt) {

    //TODO: Accumulate deltaTime and pass it to physicsWorld->dynamicsWorld->stepSimulation
    //world->physicsWorld->dynamicsWorld->stepSimulation(deltaTimeWithTimeScale, 2, deltaTime);

    physicsWorld->dynamicsWorld->stepSimulation(1.0f / 60.0f, 4, 1/240.0f);
}

glm::vec3 GameScene::BulletRaycast() {

    glm::vec3 rayStart, rayEnd;
    camera->GenerateRay(rayStart, rayEnd, 1000.0f);

    btCollisionWorld::ClosestRayResultCallback rayCallback(btVector3(rayStart.x, rayStart.y, rayStart.z), btVector3(rayEnd.x, rayEnd.y, rayEnd.z));
    rayCallback.m_collisionFilterGroup = COLLISION_GROUP_ALL | COLLISION_GROUP_CHUNKS;

    physicsWorld->dynamicsWorld->rayTest(btVector3(rayStart.x, rayStart.y, rayStart.z), btVector3(rayEnd.x, rayEnd.y, rayEnd.z), rayCallback);

    if(rayCallback.hasHit()) {
        std::cout << "Ray hit at: " << rayCallback.m_hitPointWorld.getX() << " " << rayCallback.m_hitPointWorld.getY() << " " << rayCallback.m_hitPointWorld.getZ() << std::endl;
        return glm::vec3(rayCallback.m_hitPointWorld.getX(), rayCallback.m_hitPointWorld.getY(), rayCallback.m_hitPointWorld.getZ());
    }

    return glm::vec3(0.0f, 0.0f, 0.0f);
}


void GameScene::makeBarrier(float x, float y, float z) {
    std::shared_ptr<Entity> roadBarrierEntity = std::make_shared<Entity>();


    auto roadBarrierComponent = std::make_shared<MovableObjectComponent>("../assets/road_barrier_01/road_barrier_01.obj",
                                                           "../assets/road_barrier_01/road_barrier_01_tex2.jpg",
                                                           std::vector<int>{0}, false, false, 1.0f, 0.5f);

    // roadBarrierComponent->phyMesh->meshRigidBody->setCollisionFlags(roadBarrierComponent->phyMesh->meshRigidBody->getCollisionFlags() | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);
    // physicsWorld->dynamicsWorld->addRigidBody(roadBarrierComponent->phyMesh->meshRigidBody, COLLISION_GROUP_ALL, COLLISION_GROUP_ALL);

    roadBarrierComponent->SetRenderScale(10.0f);

    roadBarrierEntity->addComponent(roadBarrierComponent);

    entities.push_back(roadBarrierEntity);

    roadBarrierComponent->SetPosition(x, y, z);
}


void GameScene::render() {

    procGameInputs();

    // Render game objects
    renderer->RenderPrep();

    auto start = std::chrono::high_resolution_clock::now();

    vSpeed = 0.0f;

    ecManager->componentSpecificPass(entities, gameInput); // ECS Component Specifics Pass
    
    ecManager->tick(entities); // ECS System Tick (All Components)

    ecManager->debugSetPlayerVehicleVelocity(vSpeed);

    auto end = std::chrono::high_resolution_clock::now();
    ecInferenceTimeMS = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    if(isBulletDebugDraw) {
      
      renderer->DebugRender();
    }

  // do multiple near/far plane passes

  // 1- 100, 100- 10000 etc play arround


    camera->Matrix(45.0f, 1.0f, 4000.0f, renderer->mainShader, "camMatrix"); //! IMPORTANT
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
    ecManager->setFreeCameraMode(followPlayerVehicle);
  }

  if (trackMouse)
  { // Let the Mouse handle the camera or not

    // Handles mouse inputs
    int mouseX, mouseY;
    Uint32 mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

    // If mouse left button is pressed
    if (mouseButtons & SDL_BUTTON(SDL_BUTTON_LEFT))
    {
      printf("Mouse Left Button Pressed\n");
      glm::vec3 rayHit = BulletRaycast();

      gameInputPtr->setDebugRaycastXYZ(rayHit.x, rayHit.y, rayHit.z);

      //makeBarrier();      

    } else {
      gameInputPtr->setDebugRaycastXYZ(0.0f, 0.0f, 0.0f);
    }


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


                if(i == 4){ // LB
                    printf("BOING");
                    camera->setFrontLook(true);
                } else {
                    camera->setFrontLook(false);
                }

            }
        }
  }

}

void GameScene::load_HighRoadHills_Map(std::shared_ptr<Entity> terrainEntity) {
    float terrainEntityScale = 15.0f;

    auto terrainRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_MAP0/MAPOI.obj",
                                                           "../src/ressources/DE_Map1/Map01_Albedo.png",
                                                            std::vector<int>{0}, true, false);

    // auto terrainRenderComponent = std::make_shared<RenderComponent>("../assets/square_island/Square_island.obj",
    //                                                        "../assets/square_island/Map_Base_Color.jpg",
    //                                                         0, true, false);

    terrainRenderComponent->SetRenderScale(terrainEntityScale);

    auto terrainRoadRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_MAP0/MAPOI.obj",
                                                           "../src/ressources/DE_MAP0/BIG_ROAD_TEX.jpg",
                                                            std::vector<int>{2}, true, false);
    terrainRoadRenderComponent->SetRenderScale(terrainEntityScale);

    auto terrainBottomRoadRenderComponent = std::make_shared<RenderComponent>("../src/ressources/DE_MAP0/MAPOI.obj",
                                                           "../src/ressources/DE_MAP0/STONE_WALL_04.jpg",
                                                            std::vector<int>{3}, false, false);
    terrainBottomRoadRenderComponent->SetRenderScale(terrainEntityScale);

    auto terrainChunks_physics_Component = std::make_shared<TerrainChunksComponent>("../src/ressources/DE_MAP0/chunks", terrainEntityScale);
    ecManager->setTerrainChunks(terrainChunks_physics_Component);

    // Add Render Components:
    terrainEntity->addComponent(terrainRenderComponent);
    terrainEntity->addComponent(terrainRoadRenderComponent);
    terrainEntity->addComponent(terrainBottomRoadRenderComponent);

    // Add Physics Component:
    terrainEntity->addComponent(terrainChunks_physics_Component);
}

void GameScene::load_SquareIsland_Map(std::shared_ptr<Entity> terrainEntity) {

    float terrainEntityScale = 260.0f;
    auto aiSplineComponent = std::make_shared<AISplineComponent>(terrainEntityScale);

    auto terrainRenderComponent = std::make_shared<RenderComponent>("../assets/square_island/Square_island.obj",
                                                           "../assets/square_island/Map_Base_Color.jpg",
                                                            std::vector<int>{0}, true, false);

    terrainRenderComponent->SetRenderScale(terrainEntityScale);

    auto terrainRoadRenderComponent = std::make_shared<RenderComponent>("../assets/square_island/Square_island.obj",
                                                           "../assets/square_island/High_Way_Tex.jpg",
                                                          std::vector<int>{2}, true, false);

    terrainRoadRenderComponent->SetRenderScale(terrainEntityScale);

    auto terrainChunks_physics_Component = std::make_shared<TerrainChunksComponent>("../assets/square_island/chunks", terrainEntityScale);
    ecManager->setTerrainChunks(terrainChunks_physics_Component);

    // Render Components:
    terrainEntity->addComponent(terrainRenderComponent);
    terrainEntity->addComponent(terrainRoadRenderComponent);
    terrainEntity->addComponent(aiSplineComponent);

    ecManager->setAISpline(aiSplineComponent);

    // Physics Component:
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
                printf( "Warning: Unable4000 to open game controller! SDL Error: %s\n", SDL_GetError() );
            }

            //Load second joystick on xbox 360 controller

            printf(
              "JOYSTICK CONNECTION :-)\n"
            );
  }

  // ECS Init

  ecManager->setCamera(camera);

    // Init reference to physics singleton


    physicsWorld = PhysicsWorldSingleton::getInstance();

    //* ----------------- Terrain Entity Definition ----------------- *//

    // std::shared_ptr<Entity> terrainEntity = std::make_shared<Entity>();
    // load_HighRoadHills_Map(terrainEntity);
    // entities.push_back(terrainEntity);

    std::shared_ptr<Entity> terrainEntity2 = std::make_shared<Entity>();
    load_SquareIsland_Map(terrainEntity2);
    entities.push_back(terrainEntity2);

    //* ----------------- Vehicle Entity Definition ----------------- *//

    std::shared_ptr<Entity> playerVehicleEntity = std::make_shared<Entity>();


    auto playerVehicleComponent = std::make_shared<VehicleControlComponent>(-2670.0f, 415.0f, 3752.0f);

    auto aiVehicleComponent = std::make_shared<AIVehicleComponent>();

    ecManager->setPlayerVehicle(playerVehicleComponent);
    ecManager->setAIVehicle(aiVehicleComponent);

    playerVehicleEntity->addComponent(playerVehicleComponent);
    playerVehicleEntity->addComponent(aiVehicleComponent);

    // auto playerVehicleRenderComponent = std::make_shared<VehicleRenderComponent>("../src/ressources/volga/volga.obj", "../src/ressources/first_car_wheel.obj",
    //                                                        "../src/ressources/volga/volga.png",
    //                                                        0, true);

    auto playerVehicleRenderComponent = std::make_shared<VehicleRenderComponent>("../assets/dale_aristocrat_vehicle/source/Dale Aristocrat PS1.gltf", "../assets/dale_aristocrat_vehicle/source/Dale Aristocrat PS1.gltf",
                                                           "../assets/dale_aristocrat_vehicle/textures/DaleAristocrat_PS1_Colored.png",
                                                           std::vector<int>{1,2}, true);

    playerVehicleRenderComponent->SetRenderScale(10.0f);

    playerVehicleEntity->addComponent(playerVehicleRenderComponent);

    entities.push_back(playerVehicleEntity);

    //this->makeBarrier();

    auto skyboxEntity = std::make_shared<Entity>();

    auto skyboxRenderComponent = std::make_shared<RenderComponent>("../assets/alien_moon_skybox/source/scene.gltf",
                                                           "../assets/alien_moon_skybox/textures/Skybox_baseColor.png",
                                                           std::vector<int>{0}, false, false);

    skyboxRenderComponent->SetRenderScale(130.0f);

    //skyboxEntity->addComponent(skyboxRenderComponent);
    //entities.push_back(skyboxEntity);

    ecManager->setSkybox(skyboxRenderComponent);


    logger->log(Logger::INFO,"GameScene Loaded in " + std::to_string(entities.size()) + " entities");

    auto cylinderEntity = std::make_shared<Entity>();

    auto cylinderRenderComponent = std::make_shared<RenderCylinderComponent>();

    cylinderEntity->addComponent(cylinderRenderComponent);

    entities.push_back(cylinderEntity);
    
}

void GameScene::initECS(std::shared_ptr<SceneManager> sceneManager) {
    ecManager = std::make_unique<ECManager>();
}

void GameScene::updateScreenSize(int w, int h) {
    WIN_WIDTH = w;
    WIN_HEIGHT = h;
    camera->UpdateScreenSize(w, h);
    renderer->UpdateScreenSize(w, h);
}