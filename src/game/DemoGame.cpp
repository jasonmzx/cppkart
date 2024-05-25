#include "DemoGame.hpp"

#define WIN_WIDTH 1900
#define WIN_HEIGHT 980

DemoGame::DemoGame() {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK | SDL_INIT_AUDIO) != 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        exit(1);
    }

    window.create("JEX [1.0.0]", WIN_WIDTH, WIN_HEIGHT, false);

    //* ========== Dear ImGui Setup ==========

    if (IMGUI_MODE) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();

        // Setup Platform/Renderer backends for Imgui
        const char* glsl_version = "#version 330 core";
        if (!ImGui_ImplSDL2_InitForOpenGL(window.getWindow(), window.getSDLContext())) {
            printf("ImGui_ImplSDL2_InitForOpenGL failed\n");
            exit(1);
        }
        if (!ImGui_ImplOpenGL3_Init(glsl_version)) {
            printf("ImGui_ImplOpenGL3_Init failed\n");
            exit(1);
        }
    }

    //* ========== SceneManager Setup ==========

    printf("Initializing SceneManager\n");
    sceneManager = std::make_shared<SceneManager>();
    printf("Setting Active Scene to GameScene\n");

    std::shared_ptr<GameScene> gameScene = std::make_shared<GameScene>();
    sceneManager->setActiveScene(gameScene);

    //* ========== ECManager Setup ==========

    printf("Initializing ECManager\n");
    ecManager = std::make_unique<ECManager>();
    ecManager->setSceneManager(sceneManager);
    
}

DemoGame::~DemoGame() {
    if (IMGUI_MODE) {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();
    }
    window.close();
    SDL_Quit();
}

void DemoGame::getUpdateInput() {
    
    SDL_Event Event;
    SDL_PumpEvents(); 
    const Uint8 *KB_InputState = SDL_GetKeyboardState(NULL);

    while (SDL_PollEvent(&Event)) {
        if (IMGUI_MODE) {
            ImGui_ImplSDL2_ProcessEvent(&Event);
        }
        if (Event.type == SDL_QUIT) {
            WindowRunning = false;
        }
    }
}

void DemoGame::tick() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (IMGUI_MODE) {
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

    ecManager.get()->tick();

    window.swapWindow();
}

void DemoGame::Run() {
    auto lastFrame = std::chrono::steady_clock::now();

    while (WindowRunning) {
        auto currentFrame = std::chrono::steady_clock::now();
        auto frameTime = std::chrono::duration<float>(currentFrame - lastFrame).count();
        lastFrame = currentFrame;


        getUpdateInput();
        tick();

    }
}
