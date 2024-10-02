#include "VehicleRenderComponent.hpp"

VehicleRenderComponent::VehicleRenderComponent
(std::string modelPath, std::string wheelModelPath, std::string texPath, std::vector<int> meshIndices, bool isTexAlpha)
: RenderComponent(modelPath, texPath, meshIndices, true, isTexAlpha)
{
    // wheelModelPath = wheelModelPath;
    WheelGeom = ressources->tryGetGeometry(wheelModelPath, 3);

    wheelModelMatrices.resize(wheelCount);
}

void VehicleRenderComponent::UpdateChassisTransform(glm::vec3 glmVehiclePos, glm::quat glmVehicleRot) {
    
    glm::mat4 translation = glm::translate(glm::mat4(1.0f), glmVehiclePos);
    
    // Convert the quaternion to a rotation matrix
    glm::mat4 rotation = glm::mat4_cast(glmVehicleRot);

    // Rotate the model 90 degrees arround the Y axis, to align it with the world

    // Translate down the Y axis:
    glm::mat4 translateDown = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -1.15f, 0.0f));

    ObjmodelMatrix = translation * rotation * translateDown * glm::scale(glm::vec3(1.65f));

    // ---- Get Direction Vector ----
    
    glm::mat4 rotation3x3 = glm::mat4_cast(glmVehicleRot);

    glm::mat4 rotate90DEG_Adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); 
    glm::mat rotation3x3_90_features = rotation3x3 * rotate90DEG_Adjustment;

    glm::vec4 modelDefaultForward = glm::vec4(0.0f, 0.0f, 1.0f, 0.0f);

    glm::vec3 forward = glm::vec3(rotation3x3 * modelDefaultForward);
    object_forward = glm::normalize(forward);

    forward = object_forward * 10.0f;

    glm::vec3 right_side = glm::vec3(rotation3x3_90_features * modelDefaultForward);
    object_right = glm::normalize(right_side);

    right_side = object_right * 5.0f;

    glm::vec3 relate_right = glmVehiclePos + right_side;     
    glm::vec3 relate_left = glmVehiclePos - right_side;

    glm::vec3 relate_forward = glmVehiclePos + forward;

    relative_forward =  relate_forward;

    glm::vec3 relate_backward = glmVehiclePos - forward;

    GameGLRenderer* renderer = GameGLRenderer::getInstance();

    // Draw a line to represent the forward direction of the vehicle

    renderer->DebugDrawLine(glmVehiclePos, relate_forward,  glm::vec3(1.0f, 0.0f, 0.0f));
    renderer->DebugDrawLine(glmVehiclePos, relate_backward, glm::vec3(1.0f, 1.0f, 0.0f));
    renderer->DebugDrawLine(glmVehiclePos, relate_right, glm::vec3(1.0f, 0.0f, 1.0f));
    renderer->DebugDrawLine(glmVehiclePos, relate_left,  glm::vec3(1.0f, 0.5f, 0.5f));
}


void VehicleRenderComponent::getForwardVector(glm::vec3 &out) {
    out = relative_forward;
}

void VehicleRenderComponent::getObjectVectors(glm::vec3 &out_forward, glm::vec3 &out_right) {
    out_forward = object_forward;
    out_right = object_right;

}

void VehicleRenderComponent::UpdateWheelTransforms(VehiclePhysics* vehiclePhysics) {
    
    float wheelRadius = 1.5f;
    wheelModelMatrices.clear();

    // wheels 0 and 2 are on the left side

    for(int i = 0; i < vehiclePhysics->vehicle->getNumWheels(); i++) {

        btWheelInfo &wheelinfo = vehiclePhysics->vehicle->getWheelInfo(i);
        glm::mat4 wheelM{1.0f};
        
        wheelinfo.m_worldTransform.getOpenGLMatrix(glm::value_ptr(wheelM));

        glm::vec3 wheelCenterOffset(0.0f, 0.0f, 0.0f); // Adjust Y offset based on your model specifics
        glm::mat4 centeringTranslation = glm::translate(glm::mat4(1.0f), wheelCenterOffset);

        // Rotate 90 degrees around the Y axis to align the wheel with the world, also translate down the Y axis
        //glm::mat4 rotateAdjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));


        // Rot 180 adjustment
        glm::mat4 rot_180 = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        if (i == 0 || i == 2) {
            // Unrotate if it's the left side 
            rot_180 = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        }


        glm::mat4 wheelModelMat = wheelM * centeringTranslation * rot_180 * glm::scale(glm::vec3(wheelRadius));

        // Apply additional translation down on the y-axis
        wheelModelMat[3][1] = wheelModelMat[3][1] - 0.2f;

        wheelModelMatrices.push_back(
            wheelModelMat 
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

    Tex->Bind();

    for(int i = 0; i < 4; i++) {
        WheelGeom->Draw(modelMatrixLOC, wheelModelMatrices[i], colorUniformLOC, false);
    }

    Tex->Unbind();

    // Restore the original culling state
    glEnable(GL_CULL_FACE);
    glCullFace(currentCullFace);
    glFrontFace(currentFrontFace);

}
