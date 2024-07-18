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

void PlayerVehicleComponent::handlePlayerVehicleMoveEvent(const Event& event) {
    try {
        auto inputs = std::any_cast<std::pair<GameInput::Control, GameInput::Control>>(event.data);
        GameInput::Control acceleration = inputs.first;
        GameInput::Control steer = inputs.second;
        
        updateVehicleControl(acceleration, steer);

    } catch (const std::bad_any_cast& e) {
        Logger::getInstance()->log(Logger::ERROR, "[PlayerVehicleComponent] Failed to cast event data: " + std::string(e.what()));
    }
}

void PlayerVehicleComponent::setPlayerPositionCallback(std::function<void(float, float, float, float)> callback) {
    setPlayerVehiclePosition = callback;
}

void PlayerVehicleComponent::tick() {
    
    float pX = vehiclePhysics.getX();
    float pY = vehiclePhysics.getY();
    float pZ = vehiclePhysics.getZ();

    float velocity = vehiclePhysics.getSpeed();

    setPlayerVehiclePosition(pX, pY, pZ, velocity);

}