#include "GameInput.hpp"

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
        currentTurn = GameInput::FullVehicleTurnLeft;
    } else if (state[SDL_SCANCODE_D]) {
        currentTurn = GameInput::FullVehicleTurnRight;
    }
}

void GameInput::xboxControllerUpdateInput(const int leftX, const int leftY, const int lt, const int rt) {
    
    // Reset control states
    currentAcceleration = Idle;
    currentTurn = Idle;

    // Check for acceleration/brake

    if (rt > 1) {
        currentAcceleration = GameInput::VehicleAccelerate;
    } else if (lt > 1) {
        currentAcceleration = GameInput::VehicleBrake;
    }    

    // if (leftY > -1) {
    //     currentAcceleration = GameInput::VehicleBrake;
    // } else if (leftY < -2) {
    //     currentAcceleration = GameInput::VehicleAccelerate;
    // }
    // leftY = -1 means idle, so we don't need to check for it

    // Check for turning

    if (leftX < 0) { // Left Turn
        
        if(leftX < -32000)
            currentTurn = GameInput::HalfVehicleTurnLeft;
        else if(leftX < -16000)
            currentTurn = GameInput::QuarterVehicleTurnLeft;
        else
            currentTurn = GameInput::Idle;
    
    } else if (leftX > 0) {
        
        if(leftX > 32000)
            currentTurn = GameInput::HalfVehicleTurnRight;
        else if(leftX > 16000)
            currentTurn = GameInput::QuarterVehicleTurnRight;
        else
            currentTurn = GameInput::Idle;

    }

}

//* =========================== Debugging & Debug Inputs =========================== *//

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
                case GameInput::FullVehicleTurnLeft:
                    std::cout << "LEFT ";
                    break;
                case GameInput::FullVehicleTurnRight:
                    std::cout << "RIGHT ";
                    break;
                // Add cases for other controls
            }
        }
        std::cout << "]" << std::endl;
    }
}

void GameInput::setDebugRaycastXYZ(float x, float y, float z) {
    debugRaycastX = x;
    debugRaycastY = y;
    debugRaycastZ = z;
}