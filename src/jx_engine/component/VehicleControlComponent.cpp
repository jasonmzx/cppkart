#include "VehicleControlComponent.hpp"

VehicleControlComponent::VehicleControlComponent(float xPos, float yPos, float zPos) 
: vehiclePhysics(xPos, yPos, zPos) {} 

void VehicleControlComponent::updateVehicleControl(GameInput::Control acceleration, GameInput::Control steer) {

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

void VehicleControlComponent::handlePlayerVehicleMoveEvent(const Event& event) {
    try {
        auto inputs = std::any_cast<std::pair<GameInput::Control, GameInput::Control>>(event.data);
        GameInput::Control acceleration = inputs.first;
        GameInput::Control steer = inputs.second;
        
        updateVehicleControl(acceleration, steer);

    } catch (const std::bad_any_cast& e) {
        Logger::getInstance()->log(Logger::ERROR, "[VehicleControlComponent] Failed to cast event data: " + std::string(e.what()));
    }
}

void VehicleControlComponent::setPlayerDirectionCallback(std::function<void(float,float,float)> callback) {
    setPlayerVehicleDirection = callback; //set forward direction
}


void VehicleControlComponent::setPlayerPositionCallback(std::function<void(float, float, float, float)> callback) {
    setPlayerVehiclePosition = callback;
}

void VehicleControlComponent::tick() {
    
    float pX = vehiclePhysics.getX();
    float pY = vehiclePhysics.getY();
    float pZ = vehiclePhysics.getZ();

    glm::vec3 vPos = glm::vec3(pX, pY, pZ);

    float velocity = vehiclePhysics.getSpeed();

    setPlayerVehiclePosition(pX, pY, pZ, velocity);

    btTransform vehicleTransform = vehiclePhysics.GetTransform();
    btQuaternion vehicleRotation = vehicleTransform.getRotation();
    glm::quat gVr(vehicleRotation.w(), vehicleRotation.x(), vehicleRotation.y(), vehicleRotation.z());

    //  Convert the quaternion to a rotation matrix                                                                                                     
    glm::mat4 rotation3x3 = glm::mat4_cast(gVr); // 3 X 3 Rotation matrix (Top-Left), the bottom row & right col of matrix looks like: (0,0,0,1)

    glm::vec4 objectSpaceForward = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f); // +Z

    glm::vec3 forward = glm::vec3(rotation3x3 * objectSpaceForward);
    forward = glm::normalize(forward);

    setPlayerVehicleDirection(forward.x, forward.y, forward.z);
}

//nearestSplineVertex: -3189.981689, -1282.049683, 4.966000
