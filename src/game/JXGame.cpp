#include "JXGame.hpp"
//#include <SDL_mixer.h>

JXGame::JXGame() {


  // Initialize SDL_mixer
    // if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
    //     printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
    // }

    window.create("JEX [0.0.1]", 1820,980, false);
    window.showCursor();

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

    window.swapWindow();

    Render();
  
  }
}


void JXGame::Render() {

    glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
    
    // Clean the back buffer and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    
}