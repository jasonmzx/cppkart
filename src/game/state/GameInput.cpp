#include "GameInput.hpp"

// Updated Controls mapping to use your GameInputState::Control
const std::unordered_multimap<int, GameInputState::Control> kDefaultControls = {
    // Mapping SDL key codes to your game's control states
    {SDLK_w, GameInputState::VehicleAccelerate},
    {SDLK_UP, GameInputState::VehicleAccelerate},
    {SDLK_s, GameInputState::VehicleBrake},
    {SDLK_DOWN, GameInputState::VehicleBrake},
    {SDLK_a, GameInputState::VehicleTurnLeft},
    {SDLK_LEFT, GameInputState::VehicleTurnLeft},
    {SDLK_d, GameInputState::VehicleTurnRight},
    {SDLK_RIGHT, GameInputState::VehicleTurnRight},
    // Add more mappings as needed
};


void GameInput::keyboardUpdateInput(const SDL_Event &event) {
    if (event.type == SDL_KEYDOWN || event.type == SDL_KEYUP) {
        auto sym = event.key.keysym.sym;
        auto level = event.type == SDL_KEYDOWN ? 1.f : 0.f;
        auto& levels = gameInputState.levels;

        // Clear previous state to avoid duplicates
        activeControlsBuffer.clear();

        auto [rangeBegin, rangeEnd] = kDefaultControls.equal_range(sym);
        for (auto it = rangeBegin; it != rangeEnd; ++it) {
            levels[it->second] = level;
            if (level > 0) { // If the key is pressed
                activeControlsBuffer.push_back(it->second);
            }
        }
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
