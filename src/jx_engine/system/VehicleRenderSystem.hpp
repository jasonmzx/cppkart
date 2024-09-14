#ifndef VEHICLE_RENDER_SYSTEM_HPP
#define VEHICLE_RENDER_SYSTEM_HPP

#include "RenderSystem.hpp"
#include "jx_engine/physics/VehiclePhysics.hpp"

struct VehicleRenderComponent {

    RenderComponent chassisRenderComponent;
    RenderComponent wheelRenderComponent;

    int wheelCount = 4;
    std::vector<glm::mat4> wheelModelMatrices;

    glm::vec3 relativeForward;
    
};

class VehicleRenderSystem : public RenderSystem {
public:
    VehicleRenderSystem() = default; // just does the RendeSystem constructor

    VehicleRenderComponent buildVehicleRenderComponent(
        std::string vehicleChassisModel, std::string vcmTexPath, std::vector<int> vcmMeshIndices,
        std::string vehicleWheelModel, std::string vwmTexPath, std::vector<int> vwmMeshIndices,
        float scale, bool cD, bool isTexAlpha); 
    
    void UpdateChassisModelMatrix(VehicleRenderComponent& vrc, glm::vec3 vehiclePos, glm::quat vehicleRot);
    void UpdateWheelModelMatrices(VehicleRenderComponent& vrc, VehiclePhysics* vehiclePhysics);
};

#endif // VEHICLE_RENDER_SYSTEM_HPP
