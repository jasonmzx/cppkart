#include "VehicleEntity.h"

VehicleEntity::VehicleEntity(VAO* vaoPtr, VBO* vboPtr, EBO* eboPtr, const std::vector<GLfloat>& vehicleBodyVertices, const std::vector<GLuint>& vehicleBodyIndices,
                            VAO* vaoPtr1, VBO* vboPtr1, EBO* eboPtr1, const std::vector<GLfloat>& wheelVertices, const std::vector<GLuint>& wheelIndices)
: 
    vehicleGeometry(vaoPtr, vboPtr, eboPtr, vehicleBodyVertices, vehicleBodyIndices),
    vehicleWheelGeometry(vaoPtr1, vboPtr1, eboPtr1, wheelVertices, wheelIndices)
{
    // The vehiclePhysics object is automatically instantiated when a VehicleEntity is created
}

void VehicleEntity::renderWheelGeometries(GLuint modelMatrixLocation, const btRaycastVehicle* vehicle){
    
    std::vector<glm::mat4> wheelMatrices(4);
    glm::mat4 rotate90DEG_Adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    //for (int i = 0; i < 1; i++)
     for (int i = 0; i < vehicle->getNumWheels(); i++)
    {
        btWheelInfo wheelinfo = vehicle->getWheelInfo(i);
      
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

RenderableGeometry& VehicleEntity::GetGeometry() {
    return vehicleGeometry;
}