#include "GameInput.hpp"

// Updated Controls mapping to use your GameInputState::Control
const std::unordered_multimap<int, GameInput::Control> kDefaultControls = {
    // Mapping SDL key codes to your game's control states
    {SDLK_w, GameInput::VehicleAccelerate},
    {SDLK_s, GameInput::VehicleBrake},
    {SDLK_a, GameInput::VehicleTurnLeft},
    {SDLK_d, GameInput::VehicleTurnRight},
    // Add more mappings as needed
};


void GameInput::keyboardUpdateInput(const Uint8 *state) {
    // Reset control states
    currentAcceleration = Idle;
    currentTurn = Idle;

    // Check for acceleration/brake
    if (state[SDL_SCANCODE_W]) {
        currentAcceleration = GameInput::VehicleAccelerate;
    } else if (state[SDL_SCANCODE_S]) {
        currentAcceleration = GameInput::VehicleBrake;
    }

    // Check for turning
    if (state[SDL_SCANCODE_A]) {
        currentTurn = GameInput::VehicleTurnLeft;
    } else if (state[SDL_SCANCODE_D]) {
        currentTurn = GameInput::VehicleTurnRight;
    }
}

bool printDebug = true;

void GameInput::getActiveControls() {
    if (printDebug) {
        std::cout << "[ ";
        for (const auto& control : activeControlsBuffer) {
            switch (control) {
                case GameInput::VehicleAccelerate:
                    std::cout << "VEH_ACC ";
                    break;
                case GameInput::VehicleBrake:
                    std::cout << "VEH_BRAKE ";
                    break;
                case GameInput::VehicleTurnLeft:
                    std::cout << "LEFT ";
                    break;
                case GameInput::VehicleTurnRight:
                    std::cout << "RIGHT ";
                    break;
                // Add cases for other controls
            }
        }
        std::cout << "]" << std::endl;
    }
}