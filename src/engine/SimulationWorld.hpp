#ifndef SIMULATIONWORLD_HPP
#define SIMULATIONWORLD_HPP

#include "engine/objects/SimulationObject.hpp"
#include "core/loaders/ObjModel.hpp"
#include <memory>

class SimulationWorld {

    public:
        SimulationWorld();
        std::vector<std::shared_ptr<SimulationObject>> entities;

};

#endif