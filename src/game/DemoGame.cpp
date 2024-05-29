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

  std::shared_ptr<GameScene> gameScene = std::make_shared<GameScene>(WIN_WIDTH, WIN_HEIGHT);
  gameScene->init();
  gameScene->initECS(sceneManager);
  sceneManager->setActiveScene(gameScene);
  logger->log(Logger::INFO, "GameScene created & set as active !");

  //* ========== ECManager Setup ==========
  
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

void DemoGame::getUpdateInput()
{

  SDL_Event Event;
  SDL_PumpEvents();
  const Uint8 *KB_InputState = SDL_GetKeyboardState(NULL);

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
      case SDLK_ESCAPE:
        CameraInputs = !CameraInputs;
      default:
        break;
      }
    }
    else if (Event.type == SDL_QUIT)
    {
      WindowRunning = 0;
    }
  }

  if (trackMouse)
  { // Let the Mouse handle the camera or not

    // Handles mouse inputs
    int mouseX, mouseY;
    Uint32 mouseButtons = SDL_GetMouseState(&mouseX, &mouseY);

    if (firstClick)
    {
      // Center the mouse cursor
      SDL_WarpMouseInWindow(window.getWindow(), WIN_WIDTH / 2, WIN_HEIGHT / 2);
      firstClick = false;
      SDL_ShowCursor(SDL_DISABLE); // Hide the mouse cursor
    }
    else
    {
      // SDL doesn't directly give us the previous position of the cursor,
      // so we compute the motion based on the current position and centering the cursor.
      int deltaX = mouseX - WIN_WIDTH / 2;
      int deltaY = mouseY - WIN_HEIGHT / 2;

      auto activeScene = sceneManager->getActiveScene();
      if (auto gameScene = dynamic_cast<GameScene *>(activeScene.get()))
      {
        gameScene->camera->Inputs();
        gameScene->camera->ProcessMouseLook(deltaX, deltaY);
      }
      else
      {
        // Handle the case where the cast fails, if necessary
        std::cerr << "Active scene is not a GameScene" << std::endl;
      }

      SDL_WarpMouseInWindow(window.getWindow(), WIN_WIDTH / 2, WIN_HEIGHT / 2);
    }
  }
}

void DemoGame::tick()
{

  sceneManager->getActiveScene().get()->render();

  if (IMGUI_MODE)
  {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplSDL2_NewFrame();
    ImGui::NewFrame();

    float framerate = ImGui::GetIO().Framerate;
    char formatted_fps_STR[50];
    sprintf(formatted_fps_STR, "FPS: %.1f", framerate);

    ImGui::Begin("FPS Counter");
    ImGui::Text("%s", formatted_fps_STR);
    ImGui::End();

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

    getUpdateInput();
    tick();
  }
}
