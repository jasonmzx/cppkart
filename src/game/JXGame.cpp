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

void JXGame::Run() {
  
  bool Running = true;
  bool Fullscreen = false;

  while (Running)
  {

    SDL_PumpEvents(); 
    Camera* cameraPtr = camera.get();

    SDL_Event Event;
    while (SDL_PollEvent(&Event))
    {

      //ImGui_ImplSDL2_ProcessEvent(&Event);

      if (Event.type == SDL_KEYDOWN)
      {
        switch (Event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
          Running = 0;
          break;
        default:
          break;
        }
      }
      else if (Event.type == SDL_QUIT)
      {
        Running = 0;
      }

    }
    cameraPtr->Inputs(window.getWindow());

    Render();
    
    window.swapWindow();

  }
}

void JXGame::Render() {

  renderer.get()->RenderALL();
}