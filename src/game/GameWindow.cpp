#include "GameWindow.hpp"

void GameWindow::create(const std::string& title, size_t w, size_t h, bool fullscreen) {

  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO); 

  uint32_t WindowFlags = SDL_WINDOW_OPENGL;
  
  window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
  static_cast<int>(w), static_cast<int>(h), WindowFlags);

  assert(window);

  //* ########## OpenGL & SDL Context Setup (Versioning, Settings) ############
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

  //SDL_GL_SetSwapInterval(1); // Enable V-Sync
  SDL_GL_SetSwapInterval(0); // Disable V-Sync
  
  glContext = SDL_GL_CreateContext(window);
  double prevTime = SDL_GetTicks(); // Window Tick Rate (SDL thing)

  gladLoadGLLoader(SDL_GL_GetProcAddress);
  glEnable(GL_DEPTH_TEST);



}

void GameWindow::close() {
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);

    window = nullptr;
}

void GameWindow::showCursor() {
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

void GameWindow::hideCursor() {
    SDL_SetRelativeMouseMode(SDL_TRUE);
}