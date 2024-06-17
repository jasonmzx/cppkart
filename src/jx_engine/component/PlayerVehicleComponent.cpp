#include "PlayerVehicleComponent.hpp"

PlayerVehicleComponent::PlayerVehicleComponent() : vehiclePhysics() {} 

void PlayerVehicleComponent::updateVehicleControl(GameInput::Control acceleration, GameInput::Control steer) {

    switch (acceleration) {
        case GameInput::VehicleAccelerate:
            vehiclePhysics.ApplyEngineForce(2000);
            break;
        case GameInput::VehicleBrake:
            vehiclePhysics.ApplyEngineForce(-2500);
            break;
        default: // Covers IDLE and any other unspecified cases
            vehiclePhysics.ApplyEngineForce(0);
            break;
    }

    switch(steer) {
        case GameInput::VehicleTurnLeft:
            vehiclePhysics.ApplySteer(0.03);
            break;
        case GameInput::VehicleTurnRight:
            vehiclePhysics.ApplySteer(-0.03);
            break;
        default: // Covers IDLE and any other unspecified cases
            vehiclePhysics.ApplySteer(0);
            break;
    }

}

