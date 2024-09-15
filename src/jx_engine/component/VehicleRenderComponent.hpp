#ifndef VEHICLERENDERCOMPONENT_HPP
#define VEHICLERENDERCOMPONENT_HPP

#include "RenderComponent.hpp"
#include "jx_engine/physics/VehiclePhysics.hpp"

class VehicleRenderComponent : public RenderComponent
{   
    public:
        VehicleRenderComponent
        (std::string modelPath, std::string wheelModelPath, std::string texPath, std::vector<int> meshIndices, bool isTexAlpha);

        void UpdateChassisTransform(glm::vec3 glmVehiclePos, glm::quat glmVehicleRot);
        void UpdateWheelTransforms(VehiclePhysics* vehiclePhysics);
        void DrawWheels();
        void getForwardVector(glm::vec3 &forward);
        void getObjectVectors(glm::vec3 &out_forward, glm::vec3 &out_right);

    private:
        std::shared_ptr<Geometry> WheelGeom;

        int wheelCount = 4;
        std::vector<glm::mat4> wheelModelMatrices;

        glm::vec3 relative_forward;

        glm::vec3 object_forward;
        glm::vec3 object_right;

};


#endif // VEHICLERENDERCOMPONENT_HPP