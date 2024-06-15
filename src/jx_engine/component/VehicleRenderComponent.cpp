#include "VehicleRenderComponent.hpp"

VehicleRenderComponent::VehicleRenderComponent
(std::string modelPath, std::string wheelModelPath, std::string texPath, std::shared_ptr<RenderRsrcManager> rrm, int meshIndex)
: RenderComponent(modelPath, texPath, rrm, meshIndex, true)
{
    // wheelModelPath = wheelModelPath;
    WheelGeom = ressources->tryGetGeometry(wheelModelPath, 0);

    wheelModelMatrices.resize(wheelCount);
}

void VehicleRenderComponent::getTransforms(glm::vec3 glmVehiclePos, glm::quat glmVehicleRot) {
    
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glmVehiclePos);
    
    // Convert the quaternion to a rotation matrix
    glm::mat4 rotation = glm::mat4_cast(glmVehicleRot);

    glm::mat4 rotate90DEG_Adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    ObjmodelMatrix = translation * rotation * rotate90DEG_Adjustment * glm::scale(glm::vec3(0.7f));
}