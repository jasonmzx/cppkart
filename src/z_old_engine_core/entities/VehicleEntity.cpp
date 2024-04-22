#include "VehicleEntity.h"

VehicleEntity::VehicleEntity(VAO* vaoPtr, VBO* vboPtr, EBO* eboPtr, const std::vector<GLfloat>& vehicleBodyVertices, const std::vector<GLuint>& vehicleBodyIndices,
                            VAO* vaoPtr1, VBO* vboPtr1, EBO* eboPtr1, const std::vector<GLfloat>& wheelVertices, const std::vector<GLuint>& wheelIndices)
: 
    vehicleGeometry(vaoPtr, vboPtr, eboPtr, vehicleBodyVertices, vehicleBodyIndices),
    vehicleWheelGeometry(vaoPtr1, vboPtr1, eboPtr1, wheelVertices, wheelIndices)
{
    // The vehiclePhysics object is automatically instantiated when a VehicleEntity is created
}

void VehicleEntity::renderWheelGeometries(GLuint modelMatrixLocation, const std::vector<btWheelInfo>& wheelVec){
    
    std::vector<glm::mat4> wheelMatrices(4);
    glm::mat4 rotate90DEG_Adjustment = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    for (const btWheelInfo& wheelinfo : wheelVec) {
        // Get Translation (Positioning)
        float wX = wheelinfo.m_worldTransform.getOrigin().getX();
        float wY = wheelinfo.m_worldTransform.getOrigin().getY() - 1;
        float wZ = wheelinfo.m_worldTransform.getOrigin().getZ();

        glm::mat4 wheelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(wX, wY, wZ));

        // Full transformation matrix for the wheel
        wheelMatrices.push_back(
            wheelTranslation * rotate90DEG_Adjustment * glm::scale(glm::vec3(0.5f))
        );
    }

    for ( glm::mat4 wheelMatrix : wheelMatrices)
    { vehicleWheelGeometry.Draw(modelMatrixLocation, wheelMatrix, NULL, false); }

}

RenderableGeometry& VehicleEntity::GetGeometry() {
    return vehicleGeometry;
}