#include "JXGame.hpp"
//#include <SDL_mixer.h>

#define WIN_WIDTH 1820
#define WIN_HEIGHT 980

#define IMGUI_MODE 1

JXGame::JXGame() {

    window.create("JEX [0.0.1]", 1820,980, false);
    //window.showCursor();

    camera = std::make_unique<Camera>(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 10.0f, 2.0f));
    world = std::make_unique<SimulationWorld>();
    renderer = std::make_unique<GameRenderer>(WIN_WIDTH, WIN_HEIGHT, camera.get(), world.get());
    

    //* Global Debug Variables
    bulletDebug = 0;


  // Initialize SDL_mixer
    // if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    //     printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    // }

  if(IMGUI_MODE == 1) {
        // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends for Imgui
    const char* glsl_version = "#version 330 core";
    ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), window.getSDLContext());
    ImGui_ImplOpenGL3_Init(glsl_version);
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
  }

  //! Disgusting Hack for Physics Chunks Init (Getting Shared Vertices from Geometry Object)
  RenderRsrcManager& res = renderer.get()->getRessourcePtr();
  std::shared_ptr<Geometry> terrainGeom = res.tryGetGeometry("../src/ressources/Landscape01.obj");

  if(terrainGeom == nullptr) {
    printf("Terrain Geometry not found\n");
  }

  std::vector<GLfloat> terrainVerts = terrainGeom.get()->_vertices;
  std::vector<GLuint> terrainIndices = terrainGeom.get()->_indices;

  physicsChunkManager = std::make_unique<PhysicsChunkManager>(terrainVerts, "../src/ressources/chunk_map.txt");
}

  bool Running = true;
  bool Fullscreen = false;
  bool CameraInputs = true;

void JXGame::getUpdateInput() {

    SDL_Event Event;
    SDL_PumpEvents(); 
    const Uint8 *state = SDL_GetKeyboardState(NULL);

    gameInput.keyboardUpdateInput(state);

    world.get()->updateVehicleControls(gameInput.currentAcceleration, gameInput.currentTurn);

    while (SDL_PollEvent(&Event))
    {
      if(IMGUI_MODE == 1)
        ImGui_ImplSDL2_ProcessEvent(&Event);

      if (Event.type == SDL_KEYDOWN)
      {
        switch (Event.key.keysym.sym)
        {
        case SDLK_F10:
          Running = 0;
          break;
        case SDLK_ESCAPE:
          CameraInputs = !CameraInputs;
        default:
          break;
        }
      }
      else if (Event.type == SDL_QUIT)
      {
        Running = 0;
      }


    }
}

void JXGame::Run() {
  
  //* Some debug variables

  bool freeCam = 0;

  //* Physics Timestep variables

  auto lastFrame = std::chrono::steady_clock::now();

  const float deltaTime = GAME_TIMESTEP;
  float accumulatedTime = 0.0f;

  while (Running)
  {

    Camera* cameraPtr = camera.get();

    getUpdateInput();
    
    if(IMGUI_MODE == 1) {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    float framerate = ImGui::GetIO().Framerate;
    char formatted_fps_STR[50]; 
    // Use sprintf to format the float as a string
    sprintf(formatted_fps_STR, "FPS: %f", framerate);

    ImGui::Begin("Debug Menu");

    if (ImGui::BeginTabBar("DebugTabBar")) {
        
        // Tab for FPS & Physics Debug
        if (ImGui::BeginTabItem("PERF")) {
            ImGui::Text("%s", formatted_fps_STR);

            ImGui::Checkbox("Toggle Free Camera", &freeCam);
            ImGui::Checkbox("Toggle Bullet Debug", &bulletDebug);


            ImGui::EndTabItem();
        }

        // Tab for Vehicle
        if (ImGui::BeginTabItem("Vehicle")) {

            std::string v_debug_str = world.get()->simObj->vehicle.debugStateSTR();
            ImGui::Text("%s", v_debug_str.c_str());
            
            // Checkbox toggle switch 


            ImGui::EndTabItem();
        }

        // Tab for Misc
        if (ImGui::BeginTabItem("MISC")) {
            
            //Button
            if (ImGui::Button("Export Terrain Chunk Map")) {
                
                //! Hacky way to get shared vertices from Geometry Object

                RenderRsrcManager& res = renderer.get()->getRessourcePtr();
                std::shared_ptr<Geometry> terrainGeom = res.tryGetGeometry("../src/ressources/Landscape01.obj");
                std::vector<GLfloat> terrainVerts = terrainGeom.get()->_vertices;

                GameDebugHelper::ExportVertexMappingToFile(terrainVerts, "../src/ressources/cpp_vertex_mapping.txt");

            }
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();

    }

    // Player Vehicle position (XYZ)

    float pX = world.get()->simObj->vehicle.getX();
    float pY = world.get()->simObj->vehicle.getY();
    float pZ = world.get()->simObj->vehicle.getZ();

    if(freeCam) {
      cameraPtr->Inputs(window.getWindow(),CameraInputs);
    } else {
      cameraPtr->VehicleFollowCamera(pX,pY,pZ);
    }

    //cameraPtr->Inputs(window.getWindow(),CameraInputs);
    Render();

    
    if(IMGUI_MODE == 1){
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }


    // Update Terrain Physics


    physicsChunkManager.get()->update(pX,pZ);

    
    auto currentFrame = std::chrono::steady_clock::now();
      
    auto frameTime = std::chrono::duration<float>(currentFrame - lastFrame).count();
    
    lastFrame = currentFrame;


    accumulatedTime += frameTime;

            // Clamp frameTime, so we won't freeze completely
            if (frameTime > 0.1f) {
                frameTime = 0.1f;
            }

    accumulatedTime = tickWorld(deltaTime, accumulatedTime);


    window.swapWindow();
  }
}

void JXGame::Render() {
  
  renderer.get()->RenderALL(bulletDebug);
}

float JXGame::tickWorld(const float deltaTime, float accumulatedTime) {
    
    auto deltaTimeWithTimeScale = deltaTime * 1.0f; //They got a hard-coded time scale also, and can change it for slow motion (ex. they do 0.5)

    while (accumulatedTime >= deltaTime) {
        world.get()->physicsWorld->dynamicsWorld->stepSimulation(deltaTimeWithTimeScale, 2, deltaTime);
        accumulatedTime -= deltaTime;
    }

    return accumulatedTime;
}