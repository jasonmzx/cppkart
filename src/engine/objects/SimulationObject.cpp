#include "SimulationObject.hpp"

SimulationObject::SimulationObject(std::shared_ptr<ObjModel> modelPtr, std::string texturePath)
    : model(modelPtr)
{
    texPath = texturePath;
}
