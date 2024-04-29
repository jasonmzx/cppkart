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

        //*Stuff from GTA 3 RW

        // auto up = -wheelinfo.m_wheelDirectionCS;    
        // auto right = wheelinfo.m_wheelAxleCS;
        // auto fwd = up.cross(right);

        // btQuaternion steerQ(up, 0.0f);
        // btQuaternion rollQ(right, -wheelinfo.m_rotation);

        // btMatrix3x3 basis(right[0], fwd[0], up[0], right[1], fwd[1], up[1],
        //                   right[2], fwd[2], up[2]);

        // btTransform t(
        //     btMatrix3x3(steerQ) * btMatrix3x3(rollQ) * basis,
        //     wi.m_chassisConnectionPointCS +
        //         wi.m_wheelDirectionCS * wheelinfo.m_raycastInfo.m_suspensionLength);


        // glm::mat4 wheelM{1.0f};
        // t.getOpenGLMatrix(glm::value_ptr(wheelM));

        // wheelM = glm::scale(wheelM, glm::vec3(0.5f));

        float wX = wheelinfo.m_worldTransform.getOrigin().getX();
        float wY = wheelinfo.m_worldTransform.getOrigin().getY() - 1;
        float wZ = wheelinfo.m_worldTransform.getOrigin().getZ();

        glm::mat4 wheelTranslation = glm::translate(glm::mat4(1.0f), glm::vec3(wX, wY, wZ));


        wheelMatrices.push_back(
            wheelTranslation * glm::scale(glm::vec3(0.5f))
        );
    }
}

