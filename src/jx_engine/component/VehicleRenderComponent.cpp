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

    // Rotate the model 90 degrees arround the Y axis, to align it with the world
    glm::mat4 rotate90DEG_Adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Translate down the Y axis:
    glm::mat4 translateDown = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.5f, 0.0f));

    ObjmodelMatrix = translation * rotation * rotate90DEG_Adjustment * translateDown * glm::scale(glm::vec3(0.7f));

}

void VehicleRenderComponent::UpdateWheelTransforms(VehiclePhysics* vehiclePhysics) {
    
    wheelModelMatrices.clear();

    for(int i = 0; i < vehiclePhysics->vehicle->getNumWheels(); i++) {

        btWheelInfo &wheelinfo = vehiclePhysics->vehicle->getWheelInfo(i);
        glm::mat4 wheelM{1.0f};
        
        wheelinfo.m_worldTransform.getOpenGLMatrix(glm::value_ptr(wheelM));

        glm::vec3 wheelCenterOffset(0.0f, -0.5f, 0.0f); // Adjust Y offset based on your model specifics
        glm::mat4 centeringTranslation = glm::translate(glm::mat4(1.0f), wheelCenterOffset);


        // Optionally apply rotation adjustments if necessary
        glm::mat4 rotateAdjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        wheelModelMatrices.push_back(
            wheelM * centeringTranslation * rotateAdjustment * glm::scale(glm::vec3(0.5f)) // Apply scaling last to maintain proportions
        );
    }

}

void VehicleRenderComponent::DrawWheels()
{
    // Log the model matrix for debugging
    // logger->log(Logger::INFO, "Model Matrix: ");
    // printMatrix(ObjmodelMatrix);

    // Save the current culling state
    GLboolean cullingEnabled = glIsEnabled(GL_CULL_FACE);
    
    glGetIntegerv(GL_CULL_FACE_MODE, &currentCullFace);
    glGetIntegerv(GL_FRONT_FACE, &currentFrontFace);

    // Temporarily disable face culling if cullDirection is false
    if (cullDisable) {
        glDisable(GL_CULL_FACE);
    }

    Tex.get()->Bind();

    for(int i = 0; i < 4; i++) {
        WheelGeom->Draw(modelMatrixLOC, wheelModelMatrices[i], colorUniformLOC, false);
    }

    Tex.get()->Unbind();

    // Restore the original culling state
    glEnable(GL_CULL_FACE);
    glCullFace(currentCullFace);
    glFrontFace(currentFrontFace);

}
