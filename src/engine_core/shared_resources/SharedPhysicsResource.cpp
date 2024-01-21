#include "SharedPhysicsResource.h"

void SharedPhysicsResource::UpdateVehiclePhyInfo(const vehiclePhysicsInfo& data) {
    std::lock_guard<std::mutex> lock(mutex_);
    playerVehicle = data;
}

vehiclePhysicsInfo SharedPhysicsResource::GetVehiclePhyInfo() {
    std::lock_guard<std::mutex> lock(mutex_);
    return playerVehicle;
}

void SharedPhysicsResource::UpdatePhysicsWorld(btDiscreteDynamicsWorld* physicsWorld) {
    std::lock_guard<std::mutex> lock(mutex_);
    physicsWorld_ = physicsWorld;
}

btDiscreteDynamicsWorld* SharedPhysicsResource::getPhysicsWorld() {
    std::lock_guard<std::mutex> lock(mutex_);
    return physicsWorld_;
}
