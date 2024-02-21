#ifndef SIMULATIONWORLD_HPP
#define SIMULATIONWORLD_HPP

#include <memory>

#include "engine/objects/VehicleObject.hpp"
#include "engine/state/GameInputState.hpp"

#include "core/loaders/ObjModel.hpp"

//Physics Imports:
#include "engine/physics/PhysicsWorldSingleton.h"

class SimulationWorld {

    public:
        SimulationWorld();
        PhysicsWorldSingleton *physicsWorld;
        std::vector<std::shared_ptr<SimulationObject>> entities;
        void updateWithPlayerInput(const ActiveInput& inputs);

        //* hardcoded for now global phy objs

        std::shared_ptr<VehicleObject> simObj;

};

#endif