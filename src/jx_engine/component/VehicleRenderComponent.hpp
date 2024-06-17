#ifndef VEHICLERENDERCOMPONENT_HPP
#define VEHICLERENDERCOMPONENT_HPP

#include "jx_engine/physics/VehiclePhysics.hpp"
#include "RenderComponent.hpp"

class VehicleRenderComponent : public RenderComponent
{   
    public:
        VehicleRenderComponent(std::string modelPath, std::string wheelModelPath, 
        std::string texPath, std::shared_ptr<RenderRsrcManager> rrm, int meshIndex);

        void getTransforms(glm::vec3 glmVehiclePos, glm::quat glmVehicleRot);
        void UpdateWheelTransforms(VehiclePhysics* vehiclePhysics);
        void DrawWheels();

    private:
        std::shared_ptr<Geometry> WheelGeom;

        int wheelCount = 4;
        std::vector<glm::mat4> wheelModelMatrices;

};


#endif // VEHICLERENDERCOMPONENT_HPP