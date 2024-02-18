#include "SimulationObject.hpp"

SimulationObject::SimulationObject(std::shared_ptr<ObjModel> modelPtr, std::shared_ptr<Texture> texPtr)
    : model(modelPtr), tex0(texPtr)
{
    // Initial position and rotation can be set here or left to default
}
