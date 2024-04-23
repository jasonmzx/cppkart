#ifndef SIMULATIONWORLD_HPP
#define SIMULATIONWORLD_HPP

#include <memory>

#include "engine/objects/VehicleObject.hpp"
#include "engine/state/GameInputState.hpp"

// File Loaders:
#include "core/loaders/ObjModel.hpp"
#include "core/loaders/ChunkedMapLoader.hpp"

//Physics Imports:
#include "engine/physics/PhysicsWorldSingleton.h"

class SimulationWorld {

    public:
        SimulationWorld();
        PhysicsWorldSingleton *physicsWorld;
        std::vector<std::shared_ptr<SimulationObject>> entities;
        void updateVehicleControls(GameInputState::Control accelerationControl, GameInputState::Control turnControl);

        //* hardcoded for now global phy objs

        std::shared_ptr<VehicleObject> simObj;
};

#endif