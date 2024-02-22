#include "GameInput.hpp"

// Updated Controls mapping to use your GameInputState::Control
const std::unordered_multimap<int, GameInputState::Control> kDefaultControls = {
    // Mapping SDL key codes to your game's control states
    {SDLK_w, GameInputState::VehicleAccelerate},
    {SDLK_s, GameInputState::VehicleBrake},
    {SDLK_a, GameInputState::VehicleTurnLeft},
    {SDLK_d, GameInputState::VehicleTurnRight},
    // Add more mappings as needed
};


void GameInput::keyboardUpdateInput(const Uint8 *state) {
    // Reset control states
    currentAcceleration = GameInputState::Null;
    currentTurn = GameInputState::Null;

    // Check for acceleration/brake
    if (state[SDL_SCANCODE_W]) {
        currentAcceleration = GameInputState::VehicleAccelerate;
    } else if (state[SDL_SCANCODE_S]) {
        currentAcceleration = GameInputState::VehicleBrake;
    }

    // Check for turning
    if (state[SDL_SCANCODE_A]) {
        currentTurn = GameInputState::VehicleTurnLeft;
    } else if (state[SDL_SCANCODE_D]) {
        currentTurn = GameInputState::VehicleTurnRight;
    }
}

bool printDebug = true;

ActiveInput GameInput::getActiveControls() {
    if (printDebug) {
        std::cout << "[ ";
        for (const auto& control : activeControlsBuffer) {
            switch (control) {
                case GameInputState::VehicleAccelerate:
                    std::cout << "VEH_ACC ";
                    break;
                case GameInputState::VehicleBrake:
                    std::cout << "VEH_BRAKE ";
                    break;
                case GameInputState::VehicleTurnLeft:
                    std::cout << "LEFT ";
                    break;
                case GameInputState::VehicleTurnRight:
                    std::cout << "RIGHT ";
                    break;
                // Add cases for other controls
            }
        }
        std::cout << "]" << std::endl;
    }
    return activeControlsBuffer;
}
