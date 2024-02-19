#ifndef SIMULATIONWORLD_HPP
#define SIMULATIONWORLD_HPP

#include <memory>

#include "engine/objects/SimulationObject.hpp"
#include "core/loaders/ObjModel.hpp"

//Physics Imports:
#include "engine/physics/PhysicsWorldSingleton.h"

class SimulationWorld {

    public:
        SimulationWorld();
        std::vector<std::shared_ptr<SimulationObject>> entities;

};

#endif