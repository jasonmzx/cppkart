#include "VehicleObject.hpp"


VehicleObject::VehicleObject(std::string carObjPath, std::string wObjPath, std::string texPath)
: SimulationObject(carObjPath, texPath)
{
    type = Vehicle;

    wheelObjPath = wObjPath;
}

void VehicleObject::UpdateModelMatrix() {

    btTransform vTrans = vehicle.GetTransform();
    btVector3 vehiclePosition = vTrans.getOrigin();
    btQuaternion vehicleRotation = vTrans.getRotation();
    
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(vehiclePosition.x(), vehiclePosition.y(), vehiclePosition.z()));

    glm::quat glmVehicleRotation = glm::quat(vehicleRotation.w(), vehicleRotation.x(), vehicleRotation.y(), vehicleRotation.z());

    glm::mat4 rotation = glm::mat4_cast(glmVehicleRotation);
    glm::mat4 rotate90DEG_Adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    objModelMatrix = translation * rotation * rotate90DEG_Adjustment * glm::scale(glm::vec3(0.75f));

}