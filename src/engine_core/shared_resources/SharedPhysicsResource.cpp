#include "SharedPhysicsResource.h"

void SharedPhysicsResource::UpdateVehiclePhyInfo(const vehiclePhysicsInfo& data) {
    std::lock_guard<std::mutex> lock(mutex_);
    playerVehicle = data;
}

vehiclePhysicsInfo SharedPhysicsResource::GetVehiclePhyInfo() {
    std::lock_guard<std::mutex> lock(mutex_);
    return playerVehicle;
}
