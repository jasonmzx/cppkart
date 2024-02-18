#include "SimulationObject.hpp"

SimulationObject::SimulationObject(std::shared_ptr<ObjModel> modelPtr)
    : model(modelPtr) 
{
    // Initial position and rotation can be set here or left to default
}
