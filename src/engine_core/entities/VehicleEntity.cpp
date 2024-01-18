#include "VehicleEntity.h"

VehicleEntity::VehicleEntity(VAO* vaoPtr, VBO* vboPtr, EBO* eboPtr, const std::vector<GLfloat>& vehicleBodyVertices, const std::vector<GLuint>& vehicleBodyIndices,
                            VAO* vaoPtr1, VBO* vboPtr1, EBO* eboPtr1, const std::vector<GLfloat>& wheelVertices, const std::vector<GLuint>& wheelIndices)
: 
    vehicleGeometry(vaoPtr, vboPtr, eboPtr, vehicleBodyVertices, vehicleBodyIndices),
    vehicleWheelGeometry(vaoPtr1, vboPtr1, eboPtr1, wheelVertices, wheelIndices)
{
    // The vehiclePhysics object is automatically instantiated when a VehicleEntity is created
}

void VehicleEntity::renderWheelGeometries(GLuint modelMatrixLocation){
    
    std::vector<glm::mat4> wheelMatrices(4);
    glm::mat4 rotate90DEG_Adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //for (int i = 0; i < 1; i++)
     for (int i = 0; i < vehiclePhysics.vehicle->getNumWheels(); i++)
    {
        btWheelInfo wheelinfo = vehiclePhysics.vehicle->getWheelInfo(i);
      
        //* Get Translation (Positioning)
        float wX = wheelinfo.m_worldTransform.getOrigin().getX();
        float wY = wheelinfo.m_worldTransform.getOrigin().getY()-1;
        float wZ = wheelinfo.m_worldTransform.getOrigin().getZ();

        glm::mat4 wheelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(wX,wY,wZ));

        //* Extract only the rotation around the axle (e.g., Y-axis)
        btTransform wheelTransform = wheelinfo.m_worldTransform;
        btScalar yaw, pitch, roll;
        wheelTransform.getBasis().getEulerZYX(yaw, pitch, roll);

    // Convert yaw to a quaternion (assuming yaw is around Y-axis)
    glm::quat glmYawQuat = glm::angleAxis(glm::degrees(yaw), glm::vec3(0, -1, 0)); // Convert yaw from radians to degrees

    glm::mat4 wheelRotation = glm::toMat4(glmYawQuat);

    // Full transformation matrix for the wheel
      wheelMatrices.push_back(
        wheelTranslation * wheelRotation * rotate90DEG_Adjustment * glm::scale(glm::vec3(0.5f))
        //wheelTranslation * wheelRotation  * glm::scale(glm::vec3(0.25f))
      );
    }

    for ( glm::mat4 wheelMatrix : wheelMatrices)
    { vehicleWheelGeometry.Draw(modelMatrixLocation, wheelMatrix, NULL, false); }

}

void VehicleEntity::updateVehicleControls(const uint8_t *state) {

  if (state[82]) { //SDL_SCANCODE_UP
    vehiclePhysics.ApplyEngineForce(2000);
  } else if (state[81]) { // SDL_SCANCODE_DOWN
    vehiclePhysics.ApplyEngineForce(-2500);
  } else {
    vehiclePhysics.ApplyEngineForce(0);
  }

  if (state[80]) { // SDL_SCANCODE_LEFT
    vehiclePhysics.ApplySteer(0.13);
  } else if (state[79]) { // SDL_SCANCODE_RIGHT
    vehiclePhysics.ApplySteer(-0.13);
  } else {
    vehiclePhysics.ApplySteer(0);
  }
}

VehiclePhysics& VehicleEntity::GetPhysics() {
    return vehiclePhysics;
}

RenderableGeometry& VehicleEntity::GetGeometry() {
    return vehicleGeometry;
}