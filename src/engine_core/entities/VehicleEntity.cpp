#include "VehicleEntity.h"

VehicleEntity::VehicleEntity() {
    // The vehiclePhysics object is automatically instantiated when a VehicleEntity is created
}


VehiclePhysics& VehicleEntity::GetPhysics() {
    return vehiclePhysics;
}