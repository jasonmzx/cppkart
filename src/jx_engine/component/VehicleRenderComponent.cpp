#include "VehicleRenderComponent.hpp"

VehicleRenderComponent::VehicleRenderComponent
(std::string modelPath, std::string wheelModelPath, std::string texPath, std::shared_ptr<RenderRsrcManager> rrm)
: RenderComponent(modelPath, texPath, rrm)
{
    // wheelModelPath = wheelModelPath;
    WheelGeom = ressources->tryGetGeometry(wheelModelPath);

    wheelModelMatrices.resize(wheelCount);
}

