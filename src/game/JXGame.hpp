#ifndef JXGAME_HPP
#define JXGAME_HPP

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <memory>

// ImGUI Imports:
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

// Source Imports:
#include "GameWindow.hpp"
#include "core/gl/Camera.h"
#include "engine/rendering/GameRenderer.hpp"
#include "engine/SimulationWorld.hpp"

#define GAME_TIMESTEP (1.f / 60.f)

class JXGame {

    public:
        JXGame();
        //~JXGame();

        void Run();
        void Render();

        float tickWorld(const float deltaTime, float accumulatedTime);
    private:
        GameWindow window{};

        std::unique_ptr<Camera> camera;
        std::unique_ptr<GameRenderer> renderer;
        std::unique_ptr<SimulationWorld> world;

};

#endif