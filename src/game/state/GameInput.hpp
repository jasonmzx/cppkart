#ifndef GAMEINPUT_HPP
#define GAMEINPUT_HPP


#include <SDL2/SDL.h>
#include <unordered_map>
#include <iostream>
#include <vector>

#include "engine/state/GameInputState.hpp"

class GameInput {
    public:
        void keyboardUpdateInput(const Uint8 *state);
        void controllerUpdateInput();
        ActiveInput getActiveControls();

        GameInputState::Control currentAcceleration = GameInputState::Null;
        GameInputState::Control currentTurn = GameInputState::Null;

    private:
        GameInputState gameInputState;

        ActiveInput activeControlsBuffer;
};

#endif