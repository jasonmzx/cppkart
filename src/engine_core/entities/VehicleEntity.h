#ifndef VEHICLEENTITY_CLASS_H
#define VEHICLEENTITY_CLASS_H

#include "engine_core/bullet_models/VehiclePhysics.h"

class VehicleEntity {
private:
    VehiclePhysics vehiclePhysics;

public:
    VehicleEntity();

    // Getter for direct access to VehiclePhysics
    VehiclePhysics& GetPhysics();
};

#endif