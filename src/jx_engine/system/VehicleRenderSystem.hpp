#ifndef VEHICLE_RENDER_SYSTEM_HPP
#define VEHICLE_RENDER_SYSTEM_HPP

#include <optional>

#include "RenderSystem.hpp"
#include "jx_engine/physics/VehiclePhysics.hpp"

#include "jx_engine/io/GameInput.hpp"

struct VehicleComponent {

    RenderComponent chassisRenderComponent;
    RenderComponent wheelRenderComponent;

    int wheelCount = 4;
    std::vector<glm::mat4> wheelModelMatrices;

    glm::vec3 relativeForward;

    std::optional<VehiclePhysics> vehiclePhysics;
    glm::vec3 worldPosition;

};

class VehicleRenderSystem : public RenderSystem {
public:
    VehicleRenderSystem() = default; // just does the RendeSystem constructor

    VehicleComponent buildVehicleComponent(
        std::string vehicleChassisModel, std::string vcmTexPath, std::vector<int> vcmMeshIndices,
        std::string vehicleWheelModel, std::string vwmTexPath, std::vector<int> vwmMeshIndices,
        float scale, bool cD, bool isTexAlpha); 
    
    void UpdateChassisModelMatrix(VehicleComponent& vrc);
    void UpdateWheelModelMatrices(VehicleComponent& vrc);
    void DrawVehicle(VehicleComponent& vrc);

    void updateVehicleControl(GameInput::Control acceleration, GameInput::Control steer, VehicleComponent& vrc);

};

#endif // VEHICLE_RENDER_SYSTEM_HPP
