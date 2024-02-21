#ifndef SIMULATIONWORLD_HPP
#define SIMULATIONWORLD_HPP

#include <memory>

#include "engine/objects/VehicleObject.hpp"
#include "core/loaders/ObjModel.hpp"

//Physics Imports:
#include "engine/physics/PhysicsWorldSingleton.h"

class SimulationWorld {

    public:
        SimulationWorld();
        PhysicsWorldSingleton *physicsWorld;
        std::vector<std::shared_ptr<SimulationObject>> entities;


};

#endif