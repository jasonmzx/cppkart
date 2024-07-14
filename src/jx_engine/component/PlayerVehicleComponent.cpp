#include "PlayerVehicleComponent.hpp"

PlayerVehicleComponent::PlayerVehicleComponent(float xPos, float yPos, float zPos) 
: vehiclePhysics(xPos, yPos, zPos) {} 

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
        case GameInput::FullVehicleTurnLeft:
            vehiclePhysics.ApplySteer(0.03);
            break;
        case GameInput::FullVehicleTurnRight:
            vehiclePhysics.ApplySteer(-0.03);
            break;
        case GameInput::HalfVehicleTurnLeft:
            vehiclePhysics.ApplySteer(0.015);
            break;
        case GameInput::HalfVehicleTurnRight:
            vehiclePhysics.ApplySteer(-0.015);
            break;
        case GameInput::QuarterVehicleTurnLeft:
            vehiclePhysics.ApplySteer(0.0075);
            break;
        case GameInput::QuarterVehicleTurnRight:
            vehiclePhysics.ApplySteer(-0.0075);
            break;    
        default: // Covers IDLE and any other unspecified cases
            vehiclePhysics.ApplySteer(0);
            break;
    }

}

void PlayerVehicleComponent::tick() {
    // 
}