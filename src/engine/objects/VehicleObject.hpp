#ifndef VEHICLEOBJECT_HPP
#define VEHICLEOBJECT_HPP

#include "core/loaders/ObjModel.hpp"
#include "SimulationObject.hpp"

#include "engine/physics/VehiclePhysics.hpp"

class VehicleObject : public SimulationObject {

    public:

        VehicleObject(std::string carObjPath, std::string wheelObjPath, std::string texPath);

        std::string wheelObjPath;

        VehiclePhysics vehicle;
        
        void UpdateModelMatrix();

};

#endif