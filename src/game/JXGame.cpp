#include "JXGame.hpp"
//#include <SDL_mixer.h>

#define WIN_WIDTH 1820
#define WIN_HEIGHT 980

JXGame::JXGame() {

    window.create("JEX [0.0.1]", 1820,980, false);
    window.showCursor();

    camera = std::make_unique<Camera>(WIN_WIDTH, WIN_HEIGHT, glm::vec3(0.0f, 0.0f, 2.0f));
    world = std::make_unique<SimulationWorld>();
    renderer = std::make_unique<GameRenderer>(WIN_WIDTH, WIN_HEIGHT, camera.get(), world.get());
    
  // Initialize SDL_mixer
    // if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    //     printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    // }


}

void JXGame::Run() {
  
  bool Running = true;
  bool Fullscreen = false;

  while (Running)
  {

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

    SDL_PumpEvents(); 

    Render();
    
    camera.get()->Inputs(window.getWindow());
    
    window.swapWindow();

  }
}


void JXGame::Render() {

  renderer.get()->RenderALL();
}