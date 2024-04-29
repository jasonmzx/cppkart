#include "VehicleObject.hpp"


VehicleObject::VehicleObject(std::string carObjPath, std::string wObjPath, std::string texPath)
: SimulationObject(carObjPath, texPath)
{
    type = Vehicle;

    wheelObjPath = wObjPath;

    wheelMatrixLength = 4; // N. Wheels
    wheelMatrices.resize(wheelMatrixLength); 
}

void VehicleObject::UpdateModelMatrix() {

    btTransform vTrans = vehicle.GetTransform();
    btVector3 vehiclePosition = vehicle.GetTransform().getOrigin();
    btQuaternion vehicleRotation = vTrans.getRotation();
    
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glm::vec3(vehiclePosition.x(), vehiclePosition.y() - 1.2f, vehiclePosition.z()));

    glm::quat glmVehicleRotation = glm::quat(vehicleRotation.w(), vehicleRotation.x(), vehicleRotation.y(), vehicleRotation.z());

    glm::mat4 rotation = glm::mat4_cast(glmVehicleRotation);
    glm::mat4 rotate90DEG_Adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    objModelMatrix = translation * rotation * rotate90DEG_Adjustment * glm::scale(glm::vec3(0.7f));


    // Update wheel matrices

    wheelMatrices.clear();

for (int i = 0; i < vehicle.vehicle->getNumWheels(); i++) {
    btWheelInfo &wheelinfo = vehicle.vehicle->getWheelInfo(i);

    glm::mat4 wheelM{1.0f};
    wheelinfo.m_worldTransform.getOpenGLMatrix(glm::value_ptr(wheelM));

    glm::vec3 wheelCenterOffset(0.0f, -0.5f, 0.0f); // Adjust Y offset based on your model specifics
    glm::mat4 centeringTranslation = glm::translate(glm::mat4(1.0f), wheelCenterOffset);

    // Optionally apply rotation adjustments if necessary
    glm::mat4 rotateAdjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    wheelMatrices.push_back(
        wheelM * centeringTranslation * rotateAdjustment * glm::scale(glm::vec3(0.5f)) // Apply scaling last to maintain proportions
    );
}
}

