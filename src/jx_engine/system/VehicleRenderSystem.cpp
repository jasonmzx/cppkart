#include "VehicleRenderSystem.hpp"


VehicleRenderComponent VehicleRenderSystem::buildVehicleRenderComponent(
        std::string vehicleChassisModel, std::string vcmTexPath, std::vector<int> vcmMeshIndices,
        std::string vehicleWheelModel, std::string vwmTexPath, std::vector<int> vwmMeshIndices,
        float scale, bool cD, bool isTexAlpha) 
{
    RenderComponent chassisRenderComponent = this->buildRenderComponent(vehicleChassisModel, vcmTexPath, vcmMeshIndices, scale, cD, isTexAlpha);
    RenderComponent wheelRenderComponent = this->buildRenderComponent(vehicleWheelModel, vwmTexPath, vwmMeshIndices, scale, cD, isTexAlpha);
    
    VehicleRenderComponent vrc;
    vrc.chassisRenderComponent = chassisRenderComponent;
    vrc.wheelRenderComponent = wheelRenderComponent;

    vrc.wheelCount = 4;
    vrc.wheelModelMatrices.resize(vrc.wheelCount);

    return vrc;
}

void VehicleRenderSystem::UpdateChassisModelMatrix(VehicleRenderComponent& vrc, glm::vec3 vehiclePos, glm::quat vehicleRot) 
{

    glm::mat4 translation = glm::translate(glm::mat4(1.0f), vehiclePos);
    glm::mat4 rotation = glm::mat4_cast(vehicleRot);

    // Translate down y axis
    glm::mat4 translateDown = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f));

    vrc.chassisRenderComponent.modelMatrix = translation * rotation * translateDown * glm::scale(glm::vec3(0.7f));

    //* ------------------- Get Forward, Backward and Side Vectors ------------------- *//

    // 90 Degree rotation around Y axis
    glm::mat4 rot90_adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 relative_rot90 = rotation * rot90_adjustment;

    // Model's default forward vector (+Z axis)
    glm::vec4 objectDefaultForward = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

    // Relative forward vector of the vehicle
    glm::vec3 objectForward = glm::vec3(rotation * objectDefaultForward);
    objectForward = glm::normalize(objectForward) * 10.0f; 

    glm::vec3 objectRight = glm::vec3(relative_rot90 * objectDefaultForward);
    objectRight = glm::normalize(objectRight) * 10.0f;

}

void VehicleRenderSystem::UpdateWheelModelMatrices(VehicleRenderComponent& vrc, VehiclePhysics* vehiclePhysics) 
{
    float wheelRadius = 0.5f;
    vrc.wheelModelMatrices.clear();

    for(int i = 0; i < vehiclePhysics->vehicle->getNumWheels(); i++) {

        btWheelInfo &wheelinfo = vehiclePhysics->vehicle->getWheelInfo(i);
        glm::mat4 wheelM{1.0f};
        
        wheelinfo.m_worldTransform.getOpenGLMatrix(glm::value_ptr(wheelM));

        glm::vec3 wheelCenterOffset(0.0f, (wheelRadius*-1), 0.0f); // Adjust Y offset based on your model specifics
        glm::mat4 centeringTranslation = glm::translate(glm::mat4(1.0f), wheelCenterOffset);

        // Rotate 90 degrees around the Y axis to align the wheel with the world, also translate down the Y axis
        glm::mat4 rotateAdjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        glm::mat4 wheelModelMat = wheelM * centeringTranslation * rotateAdjustment * glm::scale(glm::vec3(wheelRadius));

        // Apply additional translation down on the y-axis
        wheelModelMat[3][1] = wheelModelMat[3][1] - 0.8f;

        vrc.wheelModelMatrices.push_back(
            wheelModelMat 
        );
    }
}