#include "DemoGame.hpp"

#define WIN_WIDTH 1900
#define WIN_HEIGHT 980

DemoGame::DemoGame()
{
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) != 0)
  {
    printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
    exit(1);
  }

  window.create("JEX [1.0.0]", WIN_WIDTH, WIN_HEIGHT, false);

  //* ========== Dear ImGui Setup ==========

  if (IMGUI_MODE)
  {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends for Imgui
    const char *glsl_version = "#version 330 core";
    if (!ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), window.getSDLContext()))
    {
      printf("ImGui_ImplSDL2_InitForOpenGL failed\n");
      exit(1);
    }
    if (!ImGui_ImplOpenGL3_Init(glsl_version))
    {
      printf("ImGui_ImplOpenGL3_Init failed\n");
      exit(1);
    }
  }

  Logger* logger = Logger::getInstance();

  //* ========== SceneManager Setup ==========

  sceneManager = std::make_shared<SceneManager>();
  logger->log(Logger::INFO, "SceneManager created !");

  //* ========== GameScene & ECManager Setup ==========

  std::shared_ptr<GameScene> gameScene = std::make_shared<GameScene>(WIN_WIDTH, WIN_HEIGHT, window.getWindow());

  gameScene->initECS(sceneManager);

  gameScene->init();
  sceneManager->setActiveScene(gameScene);
  logger->log(Logger::INFO, "GameScene created & set as active !");
  
}

DemoGame::~DemoGame()
{
  if (IMGUI_MODE)
  {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
  }
  window.close();
  SDL_Quit();
}


void DemoGame::tick()
{

  SDL_Event Event;
  SDL_PumpEvents();

  // SDL Window Event

  while (SDL_PollEvent(&Event))
  {
    if (IMGUI_MODE == 1)
      ImGui_ImplSDL2_ProcessEvent(&Event);

    if (Event.type == SDL_KEYDOWN)
    {
      switch (Event.key.keysym.sym)
      {
      case SDLK_F10:
        WindowRunning = 0;
        break;
      default:
        break;
      }
    }

    if(Event.type == SDL_WINDOWEVENT)
    {
      if(Event.window.event == SDL_WINDOWEVENT_RESIZED)
      {
        int newWidth = Event.window.data1;
        int newHeight = Event.window.data2;
        sceneManager->getActiveScene()->updateScreenSize(newWidth, newHeight);
      }
    }

    if (Event.type == SDL_QUIT)
    {
      WindowRunning = 0;
    }
  }

  sceneManager->getActiveScene()->tickScene();

  if (IMGUI_MODE) // Setup, call Scene's ImGui Update & Render
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    sceneManager->getActiveScene()->updateImGui(); 

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
  }

  window.swapWindow();
}

void DemoGame::Run()
{
  auto lastFrame = std::chrono::steady_clock::now();

  while (WindowRunning)
  {
    auto currentFrame = std::chrono::steady_clock::now();
    auto frameTime = std::chrono::duration<float>(currentFrame - lastFrame).count();
    lastFrame = currentFrame;

    tick();
  }
}

