#ifndef GAMEINPUT_HPP
#define GAMEINPUT_HPP


#include <SDL2/SDL.h>
#include <unordered_map>
#include <iostream>
#include <vector>

#include "engine/state/GameInputState.hpp"

class GameInput {
    public:
        void keyboardUpdateInput(const SDL_Event& event);
        void controllerUpdateInput();
        ActiveInput getActiveControls();

    private:
        GameInputState gameInputState;
        ActiveInput activeControlsBuffer;
};

#endif