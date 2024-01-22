#include "SharedPhysicsResource.h"

void SharedPhysicsResource::UpdateVehiclePhyInfo(const vehiclePhysicsInfo& data) {
    std::lock_guard<std::mutex> lock(mutex_);
    playerVehicleBuffer[wI] = data;
}

vehiclePhysicsInfo SharedPhysicsResource::GetVehiclePhyInfo() {
    std::lock_guard<std::mutex> lock(mutex_);
    return playerVehicleBuffer[rI];
}

void SharedPhysicsResource::SwapBuffers() {
    std::lock_guard<std::mutex> lock(mutex_);
    std::swap(rI, wI); //Write becomes read
}

void SharedPhysicsResource::UpdatePhysicsWorld(btDiscreteDynamicsWorld* physicsWorld) {
    std::lock_guard<std::mutex> lock(mutex_);
    physicsWorld_ = physicsWorld;
}

btDiscreteDynamicsWorld* SharedPhysicsResource::getPhysicsWorld() {
    std::lock_guard<std::mutex> lock(mutex_);
    return physicsWorld_;
}
