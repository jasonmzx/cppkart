#include "JXGame.hpp"
//#include <SDL_mixer.h>

#define WIN_WIDTH 1820
#define WIN_HEIGHT 980

#define IMGUI_MODE 1

JXGame::JXGame() {

    window.create("JEX [0.0.1]", 1820,980, false);
    //window.showCursor();

    camera = std::make_unique<Camera>(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    world = std::make_unique<SimulationWorld>();
    renderer = std::make_unique<GameRenderer>(WIN_WIDTH, WIN_HEIGHT, camera.get(), world.get());
    
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
        if (ImGui::BeginTabItem("General")) {
            ImGui::Text("%s", formatted_fps_STR);
            ImGui::EndTabItem();
        }

        // Tab for Vehicle
        if (ImGui::BeginTabItem("Vehicle Debug")) {

            std::string v_debug_str = world.get()->simObj->vehicle.debugStateSTR();
            ImGui::Text("%s", v_debug_str.c_str());
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }
    ImGui::End();

    }

    cameraPtr->Inputs(window.getWindow(),CameraInputs);

    Render();
    
    if(IMGUI_MODE == 1){
      ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    tickWorld(0.0f, 0.0f);

    window.swapWindow();
  }
}

void JXGame::Render() {
  
  renderer.get()->RenderALL();
}

float JXGame::tickWorld(const float deltaTime, float accumulatedTime) {
    
    world.get()->physicsWorld->dynamicsWorld->stepSimulation(GAME_TIMESTEP, 2, GAME_TIMESTEP);

    return 0.0f; //TODO
}