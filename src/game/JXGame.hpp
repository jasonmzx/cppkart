#ifndef JXGAME_HPP
#define JXGAME_HPP

#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>

// ImGUI Imports:
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_opengl3.h"

// Source Imports:
#include "GameWindow.hpp"

class JXGame {

    public:
        JXGame();
        
        void Run();
        void Render();
    private:
        GameWindow window{};


};

#endif