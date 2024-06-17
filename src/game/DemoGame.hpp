// DemoGame.hpp

#ifndef DEMOGAME_HPP
#define DEMOGAME_HPP

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <memory>
#include <chrono>

// ImGUI Imports:
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"


// JX Engine Imports:
#include "jx_engine/core/SceneManager.hpp"
#include "jx_engine/entity/ECManager.hpp"
#include "jx_engine/logs/Logger.hpp"

//Game Imports:
#include "GameWindow.hpp"
#include "game/scenes/GameScene.hpp"

#define GAME_TIMESTEP (1.f / 60.f)

class DemoGame
{
public:
  DemoGame();
  ~DemoGame();
  void Run();

  GameWindow window{};

  std::shared_ptr<SceneManager> sceneManager;

  
private:
  void tick();

  // Game Runtime Booleans:

  bool WindowRunning = true;
  bool IMGUI_MODE = true;
  bool CameraInputs = true;

  // bool trackMouse = true;
  // bool firstClick = true;

  
};

#endif
